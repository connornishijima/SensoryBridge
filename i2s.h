#include <driver/i2s.h>

const i2s_config_t i2s_config = {
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
  .dma_buf_count = 2,
  .dma_buf_len = BUFFER_SIZE
};

const i2s_pin_config_t pin_config = {
  .bck_io_num   = I2S_BCLK_PIN,
  .ws_io_num    = I2S_LRCLK_PIN,
  .data_out_num = -1, // not used (only for outputs)
  .data_in_num  = I2S_DOUT_PIN
};

// Init I2S with some changes to increase proper support for our mic
void INIT_I2S() {
  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    if (debug_mode) {
      Serial.printf("Failed installing driver: %d\n", err);
    }
    while (true) {}
  }

  REG_SET_BIT(  I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT( I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    if (debug_mode) {
      Serial.printf("Failed setting pin: %d\n", err);
    }
    while (true) {}
  }
  if (debug_mode) {
    Serial.println("I2S driver installed.");
  }
}

void capture_audio() {
  i2s_history_index++;
  if (i2s_history_index >= 6) {
    i2s_history_index = 0;
  }

  i2s_read(I2S_PORT, i2s_samples_raw, BUFFER_SIZE * 4, &bytes_read, portMAX_DELAY);
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    i2s_samples[i2s_history_index][i] = (((i2s_samples_raw[i] / 1000000.0) + 110) * 128) + 3000;

    if (i2s_samples[i2s_history_index][i] > 32767) {
      i2s_samples[i2s_history_index][i] = 32767;
      warn = true;
    }
    else if (i2s_samples[i2s_history_index][i] < -32767) {
      i2s_samples[i2s_history_index][i] = -32767;
      warn = true;
    }
  }

  if (collecting_ambient_noise) {
    float frame_sum = 0.0;
    for(uint16_t i = 0; i < BUFFER_SIZE; i++){
      frame_sum += i2s_samples[i2s_history_index][i];
    }
    DC_OFFSET += (frame_sum / float(BUFFER_SIZE));
  }
}
