/*----------------------------------------
  Sensory Bridge FFT POST-PROCESS
----------------------------------------*/

ESP_fft FFT(FRAME_SIZE, SAMPLE_RATE, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

// Can return an FFT or multiplier value between two actual bins with linear interpolation
float interpolate(float index, float* array, uint16_t array_size) {
  float index_f = index * (array_size - 1);
  uint16_t index_i = (uint16_t)index_f;
  float index_f_frac = index_f - index_i;
  float left_val = array[index_i];
  float right_val = array[index_i + 1];

  if (index_i + 1 >= array_size) {
    right_val = left_val;
  }

  return (1 - index_f_frac) * array[index_i]
         + index_f_frac * array[index_i + 1];
}

void smooth_fft_graph() {
  const uint8_t blur_reach = 1; // must be > 0

  memcpy(fft_temp, fft_scaled, sizeof(float) * 128);

  for (int16_t i = 0; i < 128; i++) {
    int16_t current_index = i;
    float val_sum = fft_temp[current_index];
    uint8_t samples = 1;

    for (uint8_t b = 1; b <= blur_reach; b++) {
      int16_t neighbor_index_left = current_index - b;
      int16_t neighbor_index_right = current_index + b;

      if (neighbor_index_left < 0) {
        neighbor_index_left = 0;
      }
      if (neighbor_index_right >= 128) {
        neighbor_index_right = 128 - 1;
      }

      val_sum += fft_temp[neighbor_index_left];
      val_sum += fft_temp[neighbor_index_right];

      samples += 2;
    }

    fft_scaled[i] = val_sum / float(samples);
  }
}

void process_fft() {
  static uint32_t iter = 0;
  iter++;
  fft_max_move = 0.075 * (0.1 + CONFIG.MOOD * 0.9);

  fft_velocity = 0.0;

  fft_history_index++;
  if (fft_history_index >= fft_history_length) {
    fft_history_index = 0;
  }

  fft_output_history_index++;
  if (fft_output_history_index >= fft_output_history_length) {
    fft_output_history_index = 0;
  }

  for (uint16_t k = 0; k < FRAME_SIZE; k += 8) {
    fft_input[k + 0] = i2s_samples[i2s_history_index][k + 0];
    fft_input[k + 1] = i2s_samples[i2s_history_index][k + 1];
    fft_input[k + 2] = i2s_samples[i2s_history_index][k + 2];
    fft_input[k + 3] = i2s_samples[i2s_history_index][k + 3];
    fft_input[k + 4] = i2s_samples[i2s_history_index][k + 4];
    fft_input[k + 5] = i2s_samples[i2s_history_index][k + 5];
    fft_input[k + 6] = i2s_samples[i2s_history_index][k + 6];
    fft_input[k + 7] = i2s_samples[i2s_history_index][k + 7];
  }

  for(uint8_t i = 0; i < max_val_resolution; i++){
    fft_max_vals[i] = 0;
  }

  FFT.hammingWindow();
  FFT.execute();
  FFT.complexToMagnitude();

  for (uint16_t i = 0; i < FFT_SIZE; i++) {
    fft_output[i] = fft_output[i + 4];
  }

  if (collecting_ambient_noise == true) {
    for (uint16_t i = 0; i < 128; i++) {
      ambient_samples[i] += (fft_output[i]);
    }

    ambient_frames_collected++;
    return;
  } else {
    for (uint16_t i = 0; i < 128; i++) {
      fft_output[i] -= ambient_samples[i]*1.5;
      if (fft_output[i] < 0.0) {
        fft_output[i] = 0.0;
      }
    }
  }

  for (uint8_t i = 0; i < 128; i++) {
    float position = i / 128.0;
    float position_scaled; 
    
    if(CONFIG.LIGHTSHOW_MODE == DUET_MODE_LOG){
      position_scaled = position;
    }
    else{ // if not preserving original scale of FFT
      position_scaled = position * position;
    }

    fft_scaled[i] = interpolate(
      position_scaled,
      fft_output,
      FFT_SIZE * 0.45
    );
  }

  smooth_fft_graph();

  for (uint16_t i = 0; i < 128; i++) {
    uint8_t max_val_index = i / max_val_divisor;

    if ((fft_scaled[i]*0.75) > fft_max_vals[max_val_index]) {
      fft_max_vals[max_val_index] = (fft_scaled[i]*0.75);
    }
  }

  float max_val_global = 0.0;
  for(uint8_t i = 0; i < max_val_resolution; i++){
    if (fft_max_vals[i] > fft_max_vals_last[i]) {
      fft_max_vals[i] = 
        (fft_max_vals[i] * 0.15) + 
        (fft_max_vals_last[i] * 0.85);
      fft_max_vals_last[i] = fft_max_vals[i];
    } else {
      fft_max_vals[i] =
        (fft_max_vals[i] * 0.03) + 
        (fft_max_vals_last[i] * 0.97);
      fft_max_vals_last[i] = fft_max_vals[i];
    }

    if(fft_max_vals[i] > max_val_global){
      max_val_global = fft_max_vals[i];
    }
  }

  //max_val *= 0.75;

  for (uint16_t i = 0; i < 128; i++) {
    uint8_t max_val_index = i / max_val_divisor;
    float max_val_interpolate = interpolate(
      i / 128.0,
      fft_max_vals,
      max_val_resolution);
  
    //max_val_interpolate = 
    //  max_val_interpolate * 0.75 +
    //  max_val_global * 0.25;

    if (fft_scaled[i] > max_val_interpolate) {
      fft_scaled[i] = max_val_interpolate;
    } else if (fft_scaled[i] < 0.0) {
      fft_scaled[i] = 0.0;
    }

    fft_scaled[i] = fft_scaled[i] / max_val_interpolate;

    if (fft_scaled[i] > 1.0) {
      fft_scaled[i] = 1.0;
    } else if (fft_scaled[i] < 0.0) {
      fft_scaled[i] = 0.0;
    }
    // now in 0.0 - 1.0 range
  }

  int16_t past_index_a = fft_history_index - 1;
  int16_t past_index_b = fft_history_index - 2;
  if (past_index_a < 0) {
    past_index_a += 3;
  }
  if (past_index_b < 0) {
    past_index_b += 3;
  }

  for (uint16_t i = 0; i < 128; i++) {
    fft_history[fft_history_index][i] = fft_scaled[i];

    float velocity =
      fft_history[fft_history_index][i] - fft_history[past_index_b][i];

    if (velocity < 0.0) {
      velocity = 0.0;
    }

    fft_velocity += velocity;

    fft_history[fft_history_index][i] = fft_scaled[i];   
    
    /*
    fft_scaled[i] = (
      fft_history[fft_history_index][i] +
      fft_history[past_index_a][i] + 
      fft_history[past_index_b][i]
    ) / 3.0;
    */
    // 
  }

  for (uint16_t i = 0; i < 128; i++) {
    if (fft_scaled[i] < 0.001) {
      fft_scaled[i] = 0.001;
    }
    if (fft_output_last[i] < 0.001) {
      fft_output_last[i] = 0.001;
    }

    float fft_delta = fabs(fft_scaled[i] - fft_output_last[i]);

    if (fft_scaled[i] > fft_output_last[i]) {
      if (fft_delta > fft_max_move) {
        fft_scaled[i] = fft_output_last[i] + fft_max_move;
      }
    }
    else if (fft_scaled[i] < fft_output_last[i]) {
      if (fft_delta > (fft_max_move*2)) {
        fft_scaled[i] = fft_output_last[i] - (fft_max_move*2);
      }
    }

    if (fft_scaled[i] < 0.0) {
      fft_scaled[i] = 0.0;
    } else if (fft_scaled[i] > 1.0) {
      fft_scaled[i] = 1.0;
    }

    fft_output_last[i] = fft_scaled[i];

    fft_output_history[fft_output_history_index][i] = fft_scaled[i];
  }

  process_color_push();
}

void fft_lookahead_smoothing() {
  int16_t past_index = fft_output_history_index - 2;
  if (past_index < 0) {
    past_index += fft_output_history_length;
  }

  int16_t look_ahead_1 = fft_output_history_index - 1;
  if (look_ahead_1 < 0) {
    look_ahead_1 += fft_output_history_length;
  }

  int16_t look_ahead_2 = fft_output_history_index - 0;
  if (look_ahead_2 < 0) {
    look_ahead_2 += fft_output_history_length;
  }

  for (uint8_t i = 0; i < 128; i++) {
    bool look_ahead_1_rising = false;
    bool look_ahead_2_rising = false;

    if (fft_output_history[look_ahead_1][i] > fft_output_history[past_index][i]) {
      look_ahead_1_rising = true;
    }
    if (fft_output_history[look_ahead_2][i] > fft_output_history[look_ahead_1][i]) {
      look_ahead_2_rising = true;
    }

    if (look_ahead_1_rising != look_ahead_2_rising) {  // if spike
      fft_output_history[look_ahead_1][i] = (fft_output_history[past_index][i] + fft_output_history[look_ahead_2][i]) / 2.0;
    }

    fft_processed[i] = fft_output_history[past_index][i];
  }
}