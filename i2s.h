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
    Serial.printf("Failed installing driver: %d\n", err);
    while (true) {}
  }

  REG_SET_BIT(  I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT( I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true) {}
  }
  Serial.println("I2S driver installed.");
}

void capture_audio() {
  i2s_read(I2S_PORT, i2s_samples, BUFFER_SIZE * 4, &bytes_read, portMAX_DELAY);
}