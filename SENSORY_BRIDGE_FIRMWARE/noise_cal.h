extern void propagate_noise_reset();

void start_noise_cal() {
  noise_complete = false;
  max_waveform_val = 0;
  max_waveform_val_raw = 0;
  noise_iterations = 0;
  dc_offset_sum = 0;
  CONFIG.DC_OFFSET = 0;
  CONFIG.SWEET_SPOT_MIN_LEVEL = 0;
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    noise_samples[i] = 0;
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

void noise_cal_led_readout(){
  float noise_cal_progress = noise_iterations / 1024.0;
  uint8_t prog_led_index = NATIVE_RESOLUTION*noise_cal_progress;
  float max_val = 0.0;
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    if(noise_samples[i] > max_val){
      max_val = noise_samples[i];
    }
  }
  for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {
    if(i < prog_led_index){
      float led_level = noise_samples[i>>1] / max_val;
      leds[i] = CHSV(220, 255 * CONFIG.SATURATION, 255*led_level);
    }
    else if(i == prog_led_index){
      leds[i] = CRGB(0,255,255);
    }
    else{
      leds[i] = CRGB(0,0,0);
    }
  }

  if(noise_iterations > 768){ // fade out towards end of calibration
    uint16_t iters_left = 256-(noise_iterations-768);
    float brightness_level = iters_left / 256.0;
    brightness_level *= brightness_level;
    
    MASTER_BRIGHTNESS = brightness_level;                                                                    
  }
}
