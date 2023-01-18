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

void noise_cal_led_readout(){
  float noise_cal_progress = noise_iterations / 256.0;
  uint8_t prog_led_index = 128*noise_cal_progress;
  float max_val = 0.0;
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    if(noise_samples[i] > max_val){
      max_val = noise_samples[i];
    }
  }
  for (uint16_t i = 0; i < 128; i++) {
    if(i < prog_led_index){
      float led_level = noise_samples[i>>1] / max_val;
      CRGB out_col = CHSV(220, 255, 255*led_level);
      leds[i] = out_col;
    }
    else if(i == prog_led_index){
      leds[i] = CRGB(0,255,255);
    }
    else{
      leds[i] = CRGB(0,0,0);
    }
  }

  if(noise_iterations > 192){ // fade out towards end of calibration
    uint16_t iters_left = 64-(noise_iterations-192);
    float brightness_level = iters_left / 64.0;
    brightness_level*=brightness_level;
    MASTER_BRIGHTNESS = brightness_level;
    FastLED.setBrightness(255*MASTER_BRIGHTNESS);                                                                     
  }
}