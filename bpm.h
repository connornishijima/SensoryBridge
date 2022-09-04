void autocorrelate_bpm(){
  static float us_per_frame = 1000000.0 / FPS;
  static float last_bpm = 0;

  uint32_t t_start = micros();

  float max_corr = 0;
  uint16_t offset_frames = 0;
  bool falling = true;
  float last_corr = 240;  
  
  for(uint16_t offset = 0; offset < 240; offset+=1){
    float corr_sum = 0.0;
    for(uint16_t index = 0; index < 120; index+=1){
      float envelope = fft_vel_sum_history[index] * fft_vel_sum_history[index];
      float vel_sum_corr = (1.0 - fabs(fft_vel_sum_history[index] - fft_vel_sum_history[index+offset])) * envelope; 
      corr_sum += vel_sum_corr;
    }
    
    if(falling == false){
      if(corr_sum > max_corr){
        max_corr = corr_sum;
        offset_frames = offset;
      }
    }
    else{
      if(corr_sum > last_corr){
        falling = false;
      }
      last_corr = corr_sum;
    }
  }
  
  float offset_us = offset_frames*us_per_frame;
  float hz = 1000000.0 / offset_us;
  float bpm = hz*60;

  while(bpm < 90.0){
    bpm = bpm * 2.0;
  }

  while(bpm > 180.0){
    bpm = bpm / 2.0;
  }

  if(last_bpm == 0){
    last_bpm = bpm;
  }

  /*
  if(fabs((bpm / 2) - last_bpm) <= 5.0){
    bpm = last_bpm;
  }
  else if(fabs((bpm * 2) - last_bpm) <= 5.0){
    bpm = last_bpm;
  }
  else if(fabs((bpm * 1.33) - last_bpm) <= 5.0){
    bpm = last_bpm;
  }
  else if(fabs((bpm * 0.75) - last_bpm) <= 5.0){
    //bpm = last_bpm;
  }
  */
  
  last_bpm = bpm;

  uint32_t t_end = micros();

  Serial.print("0\t250\t");
  Serial.print(FPS);
  Serial.print("\t");
  Serial.print(t_end-t_start);
  Serial.print("\t");
  Serial.println(bpm);
  tempo_animation_frame_cap_target = (FPS / float(bpm/60.0))*4;
}
