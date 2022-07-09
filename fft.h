void  process_fft();
float interpolate_fft(float index, float* array, uint32_t array_size);
float interpolate_multiplier(uint8_t index);

void process_fft() {
  static uint32_t iter = 0;
  iter++;

  //Serial.println("-----------");
  //start_timing("FILL_BUFFER");

  fft_history_index++; // Advance in the FFT history
  if (fft_history_index >= FFT_HISTORY_LEN) {
    fft_history_index = 0;
  }

  for (uint16_t k = 0 ; k < BUFFER_SIZE ; k += 4) { // Fill FFT buffer with I2S samples (uint32_t to float)
    fft_input[k + 0] = i2s_samples[k + 0]; // 4 at a time to slightly unroll this loop for speed
    fft_input[k + 1] = i2s_samples[k + 1];
    fft_input[k + 2] = i2s_samples[k + 2];
    fft_input[k + 3] = i2s_samples[k + 3];
  }

  // FFT MAGIC
  //start_timing("HAMMING");
  FFT.hammingWindow();
  //start_timing("EXECUTE");
  FFT.execute();
  //start_timing("MAGNITUDE");
  FFT.complexToMagnitude();

  //start_timing("DIVISION BY 100");
  for (uint16_t i = 0; i <128; i++) { // Divide the result by 100, we don't need the resolution and want the headroom for noise averaging
    fft_output[i] /= 100.0;
  }

  if (collecting_ambient_noise) { // If NOISE_CAL happening now
    run_ambient_noise_calibration();
  }

  //start_timing("NOISE CAL");
  for (uint16_t i = 1; i < 128; i++) { // Apply noise calibration offset
    fft_output[i] -= fft_ambient_noise_sum[i];

    if (fft_output[i] < 0.0) {
      fft_output[i] = 0.0;
    }
  }

  //start_timing("INTERPOLATION");
  for (uint16_t i = 0; i < 150; i++) {
    float pos = i / 525.0; // Interpolate the FFT with lerp to stretch the image to the range we'd like
    final_fft[fft_history_index][i] = interpolate_fft(pos, fft_output, sizeof(fft_output));
  }

  //start_timing("DISCOVER MAX VALS");
  uint32_t max_vals[6] = {0, 0, 0, 0, 0, 0};
  for (uint8_t i = 0; i < 128; i++) {
    final_fft[fft_history_index][i] = final_fft[fft_history_index][i+5]; // skip first five bins, these are a bit too useless or sensitive for our liking

    if (final_fft[fft_history_index][i] < 0) {
      final_fft[fft_history_index][i] = 0;
    }

    for (uint8_t b = 0; b < 6; b++) { // Discover which autoranging zone this bin is in
      if (i >= bin_boundaries[b][0]-0 && i <= bin_boundaries[b][1]+0) {
        if (final_fft[fft_history_index][i] > max_vals[b]) {
          max_vals[b] = final_fft[fft_history_index][i];
        }
      }
    }
  }

  //start_timing("CALC MULTIPLIERS");
  for (uint8_t i = 0; i < 6; i++) {
    if (max_vals[i] > (FFT_CEILING - 5000)) {
      multipliers[i] = (FFT_CEILING - 5000) / float(max_vals[i]); // shorten to FFT_CEILING by autoranging if value exceeds FFT_CEILING
    }
    else {
      multipliers[i] = 1.0;
    }

    if (multipliers[i] < 0.0) {
      multipliers[i] = 0.0;
    }

    multipliers_history[multiplier_history_index][i] = multipliers[i];

    float mult_sum = 0.0;
    for (uint8_t m = 0; m < MULTIPLIER_HISTORY_LENGTH; m++) {
      mult_sum += multipliers_history[m][i];
    }
    multipliers_smoothed[i] = mult_sum / float(MULTIPLIER_HISTORY_LENGTH);
  }

  multiplier_history_index++; // Advance the multipliers' history index
  if (multiplier_history_index >= MULTIPLIER_HISTORY_LENGTH) {
    multiplier_history_index = 0;
  }

  //print_multiplier_curves(); // Debug print to Serial

  //start_timing("OUTPUT WITH MULTIPLIERS");

  for (uint8_t i = 0; i < 128; i++) {
    float bin_multiplier = interpolate_multiplier(i);
    final_fft[fft_history_index][i] *= bin_multiplier; // Gets a linear-interpolated multiplier value for this specific bin, from the 6 existing multiplier levels

    if (final_fft[fft_history_index][i] > FFT_CEILING) { // Double check we didn't exceed FFT_CEILING
      final_fft[fft_history_index][i] = FFT_CEILING;
    }

    final_fft[fft_history_index][i] = final_fft[fft_history_index][i]  / float(FFT_CEILING); // Convert the FFT bin to a 0.0 - 1.0 float range

    int16_t past_index = fft_history_index - 2;
    while (past_index < 0) {
      past_index += FFT_HISTORY_LEN;
    }

    fft_velocities[i] = (final_fft[fft_history_index][i] - final_fft[past_index][i]);

    //fft_velocities[i]*=2;

    // Mix this FFT bin with itself one run ago for our final result:
    processed_fft[i] = (final_fft[fft_history_index][i]*0.5 + final_fft[past_index][i]*0.5);

    //processed_fft[i] *= 1.2;
    if(processed_fft[i] > 1.0){
      processed_fft[i] = 1.0;
    }
  }
  // Repeat 127 more times

  //end_timing();

  multiplier_sum =
    ((multipliers_smoothed[0] + multipliers_smoothed[1] + multipliers_smoothed[2] + multipliers_smoothed[3] + multipliers_smoothed[4] + multipliers_smoothed[5]) / 6.0) * 0.04 + (multiplier_sum * 0.96);

}


// Can return an FFT value between two actual bins with linear interpolation
float interpolate_fft(float index, float* array, uint32_t array_size) {
  float index_f = index * (array_size / sizeof(float));
  uint16_t index_i = (uint16_t)index_f;
  float index_f_frac = index_f - index_i;
  float left_val = array[index_i];
  float right_val = array[index_i + 1];

/*
  if(left_val > right_val){
    return left_val;
  }
  else{
    return right_val;
  }
*/
  
  float value = (1 - index_f_frac) * array[index_i] + index_f_frac * array[index_i + 1];
  return value;
}


// Similar to interpolate_fft(), does the same for the six multiplier channels
float interpolate_multiplier(uint8_t index) {
  float pos = (index / 128.0) * 6;
  uint8_t pos_whole = uint8_t(floor(pos));
  uint8_t pos_next  = pos_whole + 1;
  float   pos_fractional = pos - float(pos_whole);

  if (pos_next > 5) {
    pos_next = 5;
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
