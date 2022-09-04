void calculate_onset_correlation(){
  int32_t retro_ticks[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint32_t position_zero = onset_pulse_history[15];
  retro_ticks[15] = position_zero;

  float max_corr_level = 0.0;
  uint32_t max_corr_bpm = 0;
  
  for(uint8_t bpm = 60; bpm <= 200; bpm += 2){
    float correlation_sum = 0;
    
    float bps = bpm / 60.0;
    float frames_per_beat = 60 / bps;
    int32_t temp_value = position_zero;
    for(uint8_t i = 0; i < 15; i++){
      temp_value -= frames_per_beat;
      retro_ticks[14-i] = temp_value;
    }

    for(uint8_t i = 0; i < 16; i++){
      uint32_t min_delta = 9999999;
      for(uint8_t x = 0; x < 16; x++){
        uint32_t frame_delta = abs(retro_ticks[i] - onset_pulse_history[x]);
        if(frame_delta < min_delta){
          min_delta = frame_delta;
        }
      }
      if(min_delta <= 4){ // falls within ~3 frames of each other
        correlation_sum += 1.0-(min_delta/4.0);
      }
    }
    correlation_sum /= 8.0;

    if(correlation_sum >= max_corr_level){
      max_corr_level = correlation_sum;
      max_corr_bpm = bpm;
    }

    if(correlation_sum >= 0.95){
      max_corr_level = correlation_sum;
      max_corr_bpm = bpm;
      break;
    }
  }

  Serial.println(max_corr_bpm);
}

void process_onset_pulse(){
  static bool onsets_ready = false;
  
  for(uint8_t i = 0; i < 15; i++){
    onset_pulse_history[i] = onset_pulse_history[i+1];
  }

  onset_pulse_history[15] = current_frame;

  if(onsets_ready == false){ // make sure 8 pulses have been detected
    onsets_ready = true;
    for(uint8_t i = 0; i < 16; i++){
      if(onset_pulse_history[i] == 0){
        onsets_ready = false;
      }
    }
  }

  if(onsets_ready){
    calculate_onset_correlation();
  }
}
