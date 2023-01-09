void start_noise_cal() {
  Serial.print("STARTING NOISE CAL...");
  noise_complete = false;
  noise_iterations = 0;
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    noise_samples[i] = 0;
  }
}

void clear_noise_cal() {
  Serial.print("CLEARING NOISE CAL...");
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    noise_samples[i] = 0;
  }
  save_config();
  save_ambient_noise_calibration();
  Serial.println("DONE");
}
