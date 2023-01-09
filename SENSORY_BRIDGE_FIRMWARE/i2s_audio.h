#include <driver/i2s.h>

const i2s_config_t i2s_config = { // Many of these settings are defined in (constants.h)
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
  .dma_buf_count = 1024 / SAMPLES_PER_CHUNK,
  .dma_buf_len = SAMPLES_PER_CHUNK
};

const i2s_pin_config_t pin_config = { // These too
  .bck_io_num   = I2S_BCLK_PIN,
  .ws_io_num    = I2S_LRCLK_PIN,
  .data_out_num = I2S_PIN_NO_CHANGE,  // not used (only for outputs)
  .data_in_num  = I2S_DIN_PIN
};

void init_i2s() {
  // Init I2S Driver
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  Serial.print("INIT I2S: ");
  Serial.println(result == ESP_OK ? PASS : FAIL);

  /* This is a previous workaround for I2S, likely unnecessary now
    // ESP32-S2 changes to help SPH0645 mic
    //REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
    //REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);
  
    // ESP32-S3 changes to help SPH0645 mic
    //REG_SET_BIT(I2S_RX_TIMING_REG(I2S_PORT), BIT(0));
    //REG_SET_BIT(I2S_RX_TIMING_REG(I2S_PORT), BIT(1));
    //REG_SET_BIT(I2S_RX_CONF1_REG(I2S_PORT), I2S_RX_MSB_SHIFT);
  */

  // Set I2S pins
  result = i2s_set_pin(I2S_PORT, &pin_config);
  Serial.print("I2S SET PINS: ");
  Serial.println(result == ESP_OK ? PASS : FAIL);
}

void acquire_sample_chunk() {
  // Read SAMPLES_PER_CHUNK samples from I2S at SAMPLE_RATE
  size_t bytes_read = 0; // should be SAMPLES_PER_CHUNK * 4 bytes
  i2s_read(I2S_PORT, raw_i2s_buf, buf_len, &bytes_read, 100);
  uint32_t samples_read = bytes_read / 2 / (I2S_BITS_PER_SAMPLE_32BIT / 8);

  if (samples_read < SAMPLES_PER_CHUNK) { // If something went wrong
    i2s_read_errors++;
    if (i2s_read_errors <= 10 || i2s_read_errors % 100 == 0) { // first 10 or every 100 if things are REALLY bad
      Serial.print("I2S READ ERROR! ");
      Serial.print(SAMPLES_PER_CHUNK - samples_read);
      Serial.println(" less samples than expected! ");
      Serial.println(i2s_read_errors);
      Serial.println(" errors so far.");
    }
  }

  // https://www.esp32.com/viewtopic.php?f=13&t=1756&start=40
  // Turns out this weird method is currently the best way to
  // get audio from a SPH0645 mic on an ESP32-Sx
  char *buf_ptr_read  = raw_i2s_buf;

  //  convert 2x 32 bit stereo -> 1 x 16 bit mono
  for (uint16_t i = 0; i < SAMPLES_PER_CHUNK; i++) {
    signed_audio[i] = (buf_ptr_read[3] << 8) + buf_ptr_read[2];
    buf_ptr_read += 2 * (I2S_BITS_PER_SAMPLE_32BIT / 8);
  }

  // Scale I2S samples and store into history
  for (uint16_t i = 0; i < SAMPLES_PER_CHUNK; i++) {
    float out_sample = signed_audio[i];

    if (out_sample > 32767) {  //clipping
      out_sample = 32767;
    } else if (out_sample < -32767) {
      out_sample = -32767;
    }

    sample_history[sample_history_index] = out_sample;
    sample_history_index++;
    if (sample_history_index >= SAMPLE_HISTORY_LENGTH) {
      sample_history_index = 0;
    }
  }
}
