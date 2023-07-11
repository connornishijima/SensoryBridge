extern void propagate_noise_reset();

void start_noise_cal() {
  noise_complete = false;
  max_waveform_val = 0;
  max_waveform_val_raw = 0;
  noise_iterations = 0;
  dc_offset_sum = 0;
  CONFIG.DC_OFFSET = 0;
  CONFIG.VU_LEVEL_FLOOR = 0.0;
  CONFIG.SWEET_SPOT_MIN_LEVEL = 0;
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    noise_samples[i] = 0;
  }
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    ui_mask[i] = 0;
  }
  USBSerial.println("STARTING NOISE CAL");
}

void clear_noise_cal() {
  propagate_noise_reset();
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    noise_samples[i] = 0;
  }
  save_config();
  save_ambient_noise_calibration();
  USBSerial.println("NOISE CAL CLEARED");
}