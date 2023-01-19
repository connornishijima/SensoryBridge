/*----------------------------------------
  Sensory Bridge I2S FUNCTIONS
  ----------------------------------------*/

#include <driver/i2s.h>

#if defined(CONFIG_IDF_TARGET_ESP32S2)
const i2s_config_t i2s_config = { // Many of these settings are defined in (constants.h)
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = CONFIG.SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .dma_buf_count = 1024 / (CONFIG.SAMPLES_PER_CHUNK*2),
  .dma_buf_len = CONFIG.SAMPLES_PER_CHUNK*2
};
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
const i2s_config_t i2s_config = { // Many of these settings are defined in (constants.h)
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = CONFIG.SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
  .dma_buf_count = 1024 / CONFIG.SAMPLES_PER_CHUNK,
  .dma_buf_len = CONFIG.SAMPLES_PER_CHUNK
};
#endif

const i2s_pin_config_t pin_config = { // These too
  .bck_io_num   = I2S_BCLK_PIN,
  .ws_io_num    = I2S_LRCLK_PIN,
  .data_out_num = -1,  // not used (only for outputs)
  .data_in_num  = I2S_DIN_PIN
};

void init_i2s() {
  // Init I2S Driver
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  Serial.print("INIT I2S: ");
  Serial.println(result == ESP_OK ? PASS : FAIL);

  // ESP32-S2 changes to help SPH0645 mic
#if defined(CONFIG_IDF_TARGET_ESP32S2)
  REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);
#endif

  // Set I2S pins
  result = i2s_set_pin(I2S_PORT, &pin_config);
  Serial.print("I2S SET PINS: ");
  Serial.println(result == ESP_OK ? PASS : FAIL);
}

void acquire_sample_chunk() {
  static uint32_t iter = 0;
  size_t bytes_read = 0;
  i2s_read(I2S_PORT, i2s_samples_raw, CONFIG.SAMPLES_PER_CHUNK * sizeof(int32_t), &bytes_read, portMAX_DELAY);

  max_waveform_val = 0.0;

  // Scale I2S samples and store into history
  for (uint16_t i = 0; i < CONFIG.SAMPLES_PER_CHUNK; i++) {
    int32_t sample = (i2s_samples_raw[i] * 0.000512) + 56000 - 5120;

    sample = sample >> 2;

    if (sample > 32767) {  //clipping
      sample = 32767;
    } else if (sample < -32767) {
      sample = -32767;
    }
    
    int16_t sample_abs = abs(sample)-(CONFIG.SWEET_SPOT_MIN_LEVEL>>1);
    if(sample_abs > max_waveform_val){
      max_waveform_val = sample_abs;
    }

    waveform[i] = sample;

    if (stream_audio == true) {
      Serial.println(sample);
    }
  }

  if(max_waveform_val > max_waveform_val_follower){
    float delta = max_waveform_val - max_waveform_val_follower;
    max_waveform_val_follower += delta*0.25;
  }
  else if(max_waveform_val < max_waveform_val_follower){
    float delta = max_waveform_val_follower - max_waveform_val;
    max_waveform_val_follower -= delta*0.0025;

    if(max_waveform_val_follower < CONFIG.SWEET_SPOT_MIN_LEVEL){
      max_waveform_val_follower = CONFIG.SWEET_SPOT_MIN_LEVEL;
    }
  }
  float waveform_peak_scaled_raw = max_waveform_val/max_waveform_val_follower;

  if(waveform_peak_scaled_raw > waveform_peak_scaled){
    float delta = waveform_peak_scaled_raw - waveform_peak_scaled;
    waveform_peak_scaled += delta*0.25;
  }
  else if(waveform_peak_scaled_raw < waveform_peak_scaled){
    float delta = waveform_peak_scaled - waveform_peak_scaled_raw;
    waveform_peak_scaled -= delta*0.25;
  }

  // Use the maximum amplitude of the captured frame to set
  // the Sweet Spot state. Think of this like a coordinate
  // space where 0 is the center LED, -1 is the left, and
  // +1 is the right. See run_sweet_spot() in led_utilities.h
  // for how this value translates to the final LED brightnesses
  if(max_waveform_val <= CONFIG.SWEET_SPOT_MIN_LEVEL){
    sweet_spot_state = -1;
  }
  else if(max_waveform_val >= CONFIG.SWEET_SPOT_MAX_LEVEL){
    sweet_spot_state = 1;
  }
  else{
    sweet_spot_state = 0;
  }
  
  for (int i = 0; i < SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK; i++) {
    sample_window[i] = sample_window[i + CONFIG.SAMPLES_PER_CHUNK];
  }
  for (int i = SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK; i < SAMPLE_HISTORY_LENGTH; i++) {
    sample_window[i] = waveform[i - (SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK)];
  }
}
