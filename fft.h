void  process_fft();
int16_t interpolate_fft(float index, int16_t* array, uint16_t array_size);
float interpolate_multiplier(uint8_t index);


void process_fft() {
  static uint32_t iter = 0;
  iter++;

  #ifdef LOG_TIMING
  Serial.println("-----------");
  start_timing("FILL_BUFFER");
  #endif

  fft_history_index++; // Advance in the FFT history
  if (fft_history_index >= FFT_HISTORY_LEN) {
    fft_history_index = 0;
  }

  for (uint16_t k = 0 ; k < BUFFER_SIZE ; k += 8) { // Fill FFT buffer with I2S samples (uint32_t to float)
    fft_input[k + 0] = i2s_samples[k + 0]; // 8 at a time to slightly unroll this loop for speed
    fft_input[k + 1] = i2s_samples[k + 1];
    fft_input[k + 2] = i2s_samples[k + 2];
    fft_input[k + 3] = i2s_samples[k + 3];
    fft_input[k + 4] = i2s_samples[k + 4];
    fft_input[k + 5] = i2s_samples[k + 5];
    fft_input[k + 6] = i2s_samples[k + 6];
    fft_input[k + 7] = i2s_samples[k + 7];
  }

  // FFT MAGIC
  start_timing("HAMMING");
  FFT.hammingWindow();
  start_timing("EXECUTE");
  FFT.execute();
  start_timing("MAGNITUDE");
  FFT.complexToMagnitude();

  for (uint16_t i = 0; i < 128; i++) {
    fft_integer[i] = fft_output[i + 6];
    //int16_t fft_gamma = int32_t(fft_integer[i] * fft_integer[i]) >> 15;
    //int32_t fft_mix = (fft_integer[i] + fft_gamma) / 2;
    //fft_integer[i] = fft_mix;
  }

  if (collecting_ambient_noise) { // If NOISE_CAL happening now
    run_ambient_noise_calibration();
  }

  start_timing("REMOVE NOISE");
  for (uint16_t i = 0; i < 128; i++) {
    fft_integer[i] -= fft_ambient_noise[i]; // Apply noise calibration offset

    if (fft_integer[i] < 0.0) {
      fft_integer[i] = 0.0;
    }
  }

  start_timing("SCALE RANGE");
  for (uint16_t i = 0; i < 128; i++) {
    scaled_fft[i] = fft_integer[i>>1];
  }

  start_timing("SCALE LINEAR");
  for (uint16_t i = 0; i < 128; i++) {
    float pos = (i / 127.0); // Correct FFT to linear scale
    final_fft[fft_history_index][i] = interpolate_fft((pos * pos), scaled_fft, 128);
  }

  start_timing("DISCOVER MAX VAL");
  int32_t max_val = 0;
  for (uint8_t i = 0; i < 128; i++) {
    if (final_fft[fft_history_index][i] < 0) {
      final_fft[fft_history_index][i] = 0;
    }

    float scale = 1.0;
    if(i <= 64){
      scale = i / 64.0;
      if(scale < 0.1){
        scale = 0.1;
      }
    }

    if ((final_fft[fft_history_index][i]*scale) > max_val) {
      max_val = final_fft[fft_history_index][i];
    }
  }

  start_timing("CALC MULTIPLIERS");
  if (max_val > (FFT_CEILING)) {
    multiplier = (FFT_CEILING) / float(max_val); // shorten to FFT_CEILING by autoranging if value exceeds FFT_CEILING
  }
  else {
    multiplier = 1.0;
  }

  if (multiplier < 0.0) {
    multiplier = 0.0;
  }

  if (multiplier < multiplier_last) {
    multiplier_smoothed = (multiplier * 0.025) + (multiplier_last * 0.975); // follow peaks UP faster than DOWN
  }
  else {
    multiplier_smoothed = (multiplier * 0.025) + (multiplier_last * 0.975); // follow peaks UP faster than DOWN
    //multiplier_smoothed = (multiplier * 0.0125) + (multiplier_last * 0.9875);
  }

  multiplier_last = multiplier_smoothed;

  start_timing("OUTPUT WITH MULTIPLIER");

  for (uint8_t i = 0; i < 128; i++) {
    final_fft[fft_history_index][i] *= multiplier_smoothed;

    if (final_fft[fft_history_index][i] > FFT_CEILING) { // Double check we didn't exceed FFT_CEILING
      final_fft[fft_history_index][i] = FFT_CEILING;
    }

    //final_fft[fft_history_index][i] = final_fft[fft_history_index][i]  / float(FFT_CEILING); // Convert the FFT bin to a 0.0 - 1.0 float range

    int16_t past_index = fft_history_index - 5;
    while (past_index < 0) {
      past_index += FFT_HISTORY_LEN;
    }

    fft_velocities[i] = (final_fft[fft_history_index][i] - final_fft[past_index][i]) / float(FFT_CEILING);
    if (fft_velocities[i] < 0.0) {
      fft_velocities[i] = 0.0;
    }

    past_index = fft_history_index - 2;
    while (past_index < 0) {
      past_index += FFT_HISTORY_LEN;
    }

    int32_t sum = 0;
    sum += final_fft[fft_history_index][i];
    sum += final_fft[past_index][i];
    sum = sum >> 1;

    processed_fft[i] = sum / float(FFT_CEILING);

    processed_fft[i] += processed_fft[i];

    //processed_fft[i] *= 1.2;
    if (processed_fft[i] > 1.0) {
      processed_fft[i] = 1.0;
    }
  }
  // Repeat 127 more times

  multiplier_sum = multiplier_smoothed; //[0] + multipliers_smoothed[1] + multipliers_smoothed[2] + multipliers_smoothed[3] + multipliers_smoothed[4] + multipliers_smoothed[5]) / 6.0) * 0.04 + (multiplier_sum * 0.96);

  end_timing();
}


// Can return an FFT value between two actual bins with linear interpolation
int16_t interpolate_fft(float index, int16_t* array, uint16_t array_size) {
  float index_f = index * (array_size - 1);
  uint16_t index_i = (uint16_t)index_f;
  float index_f_frac = index_f - index_i;
  float left_val = array[index_i];
  float right_val = array[index_i + 1];

  if (index_i + 1 >= array_size) {
    right_val = left_val;
  }

  return (1 - index_f_frac) * array[index_i] + index_f_frac * array[index_i + 1];
}

/*
// Similar to interpolate_fft(), does the same for the six multiplier channels
float interpolate_multiplier(uint8_t index) {
  float pos = (index / 127.0) * 5;
  uint8_t pos_whole = uint8_t(floor(pos));
  uint8_t pos_next  = pos_whole + 1;
  float   pos_fractional = pos - float(pos_whole);

  if (pos_next > 11) {
    pos_next = 11;
  }

  float mult_a = multipliers_smoothed[pos_whole];
  float mult_b = multipliers_smoothed[pos_next];

  float mult_local = ( mult_a * (1.0 - pos_fractional) + mult_b * (pos_fractional) );

  float mult_min = 1.0;
  for (uint8_t i = 0; i < 6; i++) {
    if (multipliers_smoothed[i] < mult_min) {
      mult_min = multipliers_smoothed[i];
    }
  }

  return (mult_min * AUTORANGE_MIX) + (mult_local * (1.0 - AUTORANGE_MIX));
}
*/