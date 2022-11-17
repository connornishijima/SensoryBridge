/*----------------------------------------
  Sensory Bridge NOISE CAL FUNCTIONS
----------------------------------------*/

void calculate_ambient_noise_average() {
  float avg_value = 0.0;

  for (uint16_t i = 0; i < 128; i++) {
    ambient_samples[i] /= ambient_frames_collected;
    avg_value += ambient_samples[i];
  }
  avg_value /= 128.0;

  CONFIG.DC_OFFSET /= float(ambient_frames_collected);
  CONFIG.WAVEFORM_NOISE_FLOOR /= float(ambient_frames_collected);
}

void start_noise_cal() {
  for (uint8_t i = 0; i < 128; i++) {
    ambient_samples[i] = 0;
  }
  clear_all_led_buffers();
  show_leds();
  ambient_start_time = millis();
  ambient_solve_time = ambient_start_time + 3000;
  ambient_frames_collected = 0;
}

void show_progress_bar(float progress, CRGB col) {
  if(millis() < ambient_start_time+50){
    progress = 0.0;
  }

  for (uint8_t i = 0; i < 128; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  int16_t end_led = 127 * progress;
  float max_val = 0.0;
  for (uint8_t i = 0; i < 128; i++) {
    if (ambient_samples[i/2] > max_val) {
      max_val = ambient_samples[i/2];
    }
  }
  for (int i = 0; i <= end_led; i++) {
    if (i == end_led) {
      leds[i] = col;
    } else{
      float out_val = (ambient_samples[i/2] / max_val);
      leds[i] = CHSV(16, 255, 255 * out_val);
    }
  }
}

void run_noise_cal(uint32_t t_now) {
  if (collecting_ambient_noise == true) {
    float progress = (t_now - ambient_start_time) / float(ambient_solve_time - ambient_start_time);
    show_progress_bar(progress, CRGB(0, 255, 255));
    if (t_now >= ambient_solve_time) {
      collecting_ambient_noise = false;
      calculate_ambient_noise_average();
      save_ambient_noise_calibration();
      save_config();
    }

    show_leds();
  }
}

void clear_noise_cal() {
  for (uint16_t i = 0; i < 128; i++) {
    ambient_samples[i] = 0;
  }
  save_config();
  save_ambient_noise_calibration();
  Serial.println("CLEARED AMBIENT NOISE CALIBRATION!");
}