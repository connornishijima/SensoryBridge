/*----------------------------------------
  Sensory Bridge I2S FUNCTIONS
----------------------------------------*/

#include <driver/i2s.h>

const i2s_config_t i2s_config = {
  .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .dma_buf_count = 2,
  .dma_buf_len = BUFFER_SIZE
};

const i2s_pin_config_t pin_config = {
  .bck_io_num = I2S_BCLK_PIN,
  .ws_io_num = I2S_LRCLK_PIN,
  .data_out_num = -1,  // not used (only for outputs)
  .data_in_num = I2S_DOUT_PIN
};

// Init I2S with some changes to increase the usability of our weird mic
void init_i2s() {
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  Serial.print("INIT I2S: ");
  Serial.println(result == ESP_OK ? "PASS" : "FAIL");

  // ESP32-S2 changes to help SPH0645 mic
  REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

  result = i2s_set_pin(I2S_PORT, &pin_config);
  Serial.print("I2S SET PINS: ");
  Serial.println(result == ESP_OK ? "PASS" : "FAIL");
}

void capture_audio() {
  size_t bytes_read = 0;
  i2s_history_index++;
  if (i2s_history_index >= 4) {
    i2s_history_index = 0;
  }

  i2s_read(I2S_PORT, i2s_samples_raw, BUFFER_SIZE * sizeof(uint32_t), &bytes_read, portMAX_DELAY);

  float min_sample = 1000000;
  float max_sample = -1000000;
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    float out_sample = ((i2s_samples_raw[i] / 1000000.0) + 110) * 128 - 250;

    if (out_sample > 32767) {  //clipping
      out_sample = 32767;
    } else if (out_sample < -32767) {
      out_sample = -32767;
    }

    if (out_sample < min_sample) {
      min_sample = out_sample;
    }
    if (out_sample > max_sample) {
      max_sample = out_sample;
    }

    i2s_samples[i2s_history_index][(BUFFER_SIZE * i2s_buffer_index) + i] = out_sample;
  }

  float max_value;
  if (abs(max_sample) > abs(min_sample)) {
    max_value = abs(max_sample);
  } else {
    max_value = abs(min_sample);
  }

  if (collecting_ambient_noise) { // If studying waveform noise floor / DC offset
    float frame_sum = 0.0;
    for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
      frame_sum += i2s_samples[i2s_history_index][(BUFFER_SIZE * i2s_buffer_index) + i];
    }
    CONFIG.DC_OFFSET += (frame_sum / float(BUFFER_SIZE));
    CONFIG.WAVEFORM_NOISE_FLOOR += max_value;
  } else { // If already aware of waveform noise floor / DC offset
    uint16_t sweet_spot_low_end = CONFIG.WAVEFORM_NOISE_FLOOR + 100;  // -- Near noise floor
    uint16_t sweet_spot_high_end = 30000;                             // --------------------- Near clipping

    sweet_spot_rough = 1;

    if (max_value < sweet_spot_low_end) {
      sweet_spot_rough = 0;
    } else if (max_value > sweet_spot_high_end) {
      sweet_spot_rough = 2;
    }
  }

  i2s_buffer_index++;
  if (i2s_buffer_index >= BUFFER_COUNT) {
    i2s_buffer_index = 0;
  }
}