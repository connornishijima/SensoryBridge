/*----------------------------------------
  Sensory Bridge I2S FUNCTIONS
  ----------------------------------------*/

#include <driver/i2s.h>

const i2s_config_t i2s_config = { // Many of these settings are defined in (constants.h)
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = CONFIG.SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .dma_buf_count = 2,
  .dma_buf_len = CONFIG.SAMPLES_PER_CHUNK
};

const i2s_pin_config_t pin_config = { // These too
  .bck_io_num   = I2S_BCLK_PIN,
  .ws_io_num    = I2S_LRCLK_PIN,
  .data_out_num = -1,  // not used (only for outputs)
  .data_in_num  = I2S_DIN_PIN
};

void init_i2s() {
  // Init I2S Driver
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  USBSerial.print("INIT I2S: ");
  USBSerial.println(result == ESP_OK ? PASS : FAIL);

  // ESP32-S2 changes to help SPH0645 mic
#if defined(CONFIG_IDF_TARGET_ESP32S2)
  REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);
#endif

  // Set I2S pins
  result = i2s_set_pin(I2S_PORT, &pin_config);
  USBSerial.print("I2S SET PINS: ");
  USBSerial.println(result == ESP_OK ? PASS : FAIL);
}

void acquire_sample_chunk(uint32_t t_now) {
  static int8_t   sweet_spot_state_last = 0;
  static bool     silence_temp = false;
  static uint32_t silence_switched = 0;
  static float    silent_scale_last = 1.0;

  size_t bytes_read = 0;
  i2s_read(I2S_PORT, i2s_samples_raw, CONFIG.SAMPLES_PER_CHUNK * sizeof(int32_t), &bytes_read, portMAX_DELAY);

  max_waveform_val = 0.0;
  max_waveform_val_raw = 0.0;
  waveform_history_index++;
  if (waveform_history_index >= 4) {
    waveform_history_index = 0;
  }

  // Scale I2S samples and store into history
  for (uint16_t i = 0; i < CONFIG.SAMPLES_PER_CHUNK; i++) {
    int32_t sample = (i2s_samples_raw[i] * 0.000512) + 56000 - 5120;

    sample = sample >> 2; // Helps prevent overflow in fixed-point math coming up

    sample *= CONFIG.SENSITIVITY; // Set sensitivity gain

    if (sample > 32767) { // clipping
      sample = 32767;
    } else if (sample < -32767) {
      sample = -32767;
    }

    waveform[i] = sample - CONFIG.DC_OFFSET;
    waveform_history[waveform_history_index][i] = waveform[i];

    uint32_t sample_abs = abs(sample);
    if (sample_abs > max_waveform_val_raw) {
      max_waveform_val_raw = sample_abs;
    }
  }

  if (stream_audio == true) {
    USBSerial.print("sbs((audio=");
    for (uint16_t i = 0; i < CONFIG.SAMPLES_PER_CHUNK; i++) {
      USBSerial.print(waveform[i]);
      if (i < CONFIG.SAMPLES_PER_CHUNK - 1) {
        USBSerial.print(',');
      }
    }
    USBSerial.println("))");
  }

  if (noise_complete == false) {
    dc_offset_sum += waveform[0];

    silent_scale = 1.0; // Force LEDs on during calibration

    if (noise_iterations >= 64 && noise_iterations <= 192) { // sample in the middle of noise cal
      if (max_waveform_val_raw*1.10 > CONFIG.SWEET_SPOT_MIN_LEVEL) { // Sweet Spot Min threshold should be the silence level + 15%
        CONFIG.SWEET_SPOT_MIN_LEVEL = max_waveform_val_raw*1.10;
      }
    }
  }
  else {
    max_waveform_val = (max_waveform_val_raw - (CONFIG.SWEET_SPOT_MIN_LEVEL));
    
    if (max_waveform_val > max_waveform_val_follower) {
      float delta = max_waveform_val - max_waveform_val_follower;
      max_waveform_val_follower += delta * 0.25;
    }
    else if (max_waveform_val < max_waveform_val_follower) {
      float delta = max_waveform_val_follower - max_waveform_val;
      max_waveform_val_follower -= delta * 0.005;

      if (max_waveform_val_follower < CONFIG.SWEET_SPOT_MIN_LEVEL) {
        max_waveform_val_follower = CONFIG.SWEET_SPOT_MIN_LEVEL;
      }
    }
    float waveform_peak_scaled_raw = max_waveform_val / max_waveform_val_follower;

    if (waveform_peak_scaled_raw > waveform_peak_scaled) {
      float delta = waveform_peak_scaled_raw - waveform_peak_scaled;
      waveform_peak_scaled += delta * 0.25;
    }
    else if (waveform_peak_scaled_raw < waveform_peak_scaled) {
      float delta = waveform_peak_scaled - waveform_peak_scaled_raw;
      waveform_peak_scaled -= delta * 0.25;
    }

    // Use the maximum amplitude of the captured frame to set
    // the Sweet Spot state. Think of this like a coordinate
    // space where 0 is the center LED, -1 is the left, and
    // +1 is the right. See run_sweet_spot() in led_utilities.h
    // for how this value translates to the final LED brightnesses

    if (max_waveform_val_raw <= CONFIG.SWEET_SPOT_MIN_LEVEL * 1.10) {
      sweet_spot_state = -1;
      if (sweet_spot_state_last != -1) { // Just became silent
        silence_temp = true;
        silence_switched = t_now;
      }
    }
    else if (max_waveform_val_raw >= CONFIG.SWEET_SPOT_MAX_LEVEL) {
      sweet_spot_state = 1;
      if (sweet_spot_state_last != 1) { // No longer silent
        silence_temp = false;
        silence_switched = t_now;
      }
    }
    else {
      sweet_spot_state = 0;
      if (sweet_spot_state_last != 0) { // No longer silent
        silence_temp = false;
        silence_switched = t_now;
      }
    }

    if (silence_temp == true) {
      if (t_now - silence_switched >= 10000) {
        silence = true;
      }
    }
    else {
      silence = false;
    }


    if (CONFIG.STANDBY_DIMMING == true) {
      // Make silent_scale more slowly track instant change in reality
      float silent_scale_raw = 1.0 - silence; // Turn off when quiet
      silent_scale = silent_scale_raw * 0.1 + silent_scale_last * 0.9;
      silent_scale_last = silent_scale;
    }
    else {
      silent_scale = 1.0;
    }

    for (int i = 0; i < SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK; i++) {
      sample_window[i] = sample_window[i + CONFIG.SAMPLES_PER_CHUNK];
    }
    for (int i = SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK; i < SAMPLE_HISTORY_LENGTH; i++) {
      sample_window[i] = waveform[i - (SAMPLE_HISTORY_LENGTH - CONFIG.SAMPLES_PER_CHUNK)];
    }

    for(uint16_t i = 0; i < CONFIG.SAMPLES_PER_CHUNK; i++){
      waveform_fixed_point[i] = SQ15x16(waveform[i]) / SQ15x16(32768.0);
    }

    sweet_spot_state_last = sweet_spot_state;
  }
}

void calculate_vu() {
    /*
    Calculates perceived audio loudness or Volume Unit (VU). Uses root mean square (RMS) method 
    for accurate representation of perceived loudness and incorporates a noise floor calibration.
    If calibration is active, updates noise floor level. If not, subtracts the noise floor from
    the calculated volume and normalizes the volume level.

    Parameters:
    - audio_samples[]: Audio samples to process.
    - sample_count: Number of samples in audio_samples array.

    Global variables:
    - audio_vu_level: Current VU level.
    - audio_vu_level_last: Last calculated VU level.
    - CONFIG.VU_LEVEL_FLOOR: Quietest level considered as audio signal.
    - audio_vu_level_average: Average of the current and the last VU level.
    - noise_cal_active: Indicator of active noise floor calibration.
    */

    SQ15x16 sample_count = CONFIG.SAMPLES_PER_CHUNK;

    // Store last volume level
    audio_vu_level_last = audio_vu_level;
    
    SQ15x16 sum = 0.0;

    // Compute sum of squares
    for(uint16_t i = 0; i < sample_count; i++) {
        sum += waveform_fixed_point[i] * waveform_fixed_point[i];
    }

    // Compute RMS
    SQ15x16 rms = sqrt(float(sum / sample_count));
    
    // Update volume level
    audio_vu_level = rms;
    
    // Check noise calibration
    if(noise_complete == false){
        // If volume level exceeds noise floor, update noise floor
        if(float(audio_vu_level*1.5) > CONFIG.VU_LEVEL_FLOOR){
            CONFIG.VU_LEVEL_FLOOR = float(audio_vu_level*1.5);
        }
    }
    else{
        // Subtract noise floor from volume level
        audio_vu_level -= CONFIG.VU_LEVEL_FLOOR;
        
        // Zero out negative volume
        if(audio_vu_level < 0.0){
            audio_vu_level = 0.0;
        }
       
        // Normalize volume level
        audio_vu_level /= (1.0-CONFIG.VU_LEVEL_FLOOR);
    }

    // Compute average volume level
    audio_vu_level_average = (audio_vu_level+audio_vu_level_last) / SQ15x16(2.0);
}
