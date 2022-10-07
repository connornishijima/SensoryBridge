void duet_mode();
void bloom_mode();

void duet_mode() {
  for (uint16_t i = 0; i < 128; i++) {
    float fft_val = processed_fft[i];

    float out_val = ( fft_val ) * 255.0;

    if (out_val < 127) {
      //out_val = 0;
    }
    else {
      //out_val = 255;
    }

    if (out_val < 0) {
      out_val = 0;
    }

    if (out_val > 255.0) {
      out_val = 255.0;
    }

    float final_val = (out_val * (1.0 - SMOOTHING)) + (last_fft_frame[i] * (SMOOTHING));

    if (final_val < 0.0) {
      final_val = 0.0;
    }
    else if (final_val > 255.0) {
      final_val = 255.0;
    }

    uint8_t smooth_threshold = 100 - (100 * SMOOTHING);
    if (abs(final_val - last_fft_frame[i]) > smooth_threshold) {
      if (final_val > last_fft_frame[i]) {
        final_val = last_fft_frame[i] + smooth_threshold;
      }
      else if (final_val < last_fft_frame[i]) {
        final_val = last_fft_frame[i] - smooth_threshold;
      }
    }

    last_fft_frame[i] = final_val;

    float gamma_val = uint16_t( (uint8_t(final_val) * uint8_t(final_val)) >> 8 );
    final_val = gamma_val;

    //final_val *= 0.75;
    //final_val += 255*0.25;

    float color_val = hue - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.8) + (i * hue_shift_amount) - (32 * (final_val / 255.0));

    leds[i] = ColorFromPalette( current_palette, uint8_t(color_val), final_val, LINEARBLEND );
    //leds[i] = CHSV(color_val, 255, final_val);
  }

  //blur1d( leds, NUM_LEDS, 2 );
  process_color_push();
}


void bloom_mode() {
  static uint32_t iter = 0;
  iter++;
  float fft_sum = 0.0;
  for (uint8_t i = 0; i < 128; i++) {
    fft_sum += processed_fft[i];
  }
  fft_sum /= 128.0;

  fft_sum *= 1.5;
  if(fft_sum > 1.0){
    fft_sum = 1.0;
  }

  fft_sum = fft_sum * fft_sum;

  if(iter % 2 == 0){
    for (int16_t i = 127; i > 0; i--) {
      leds_temp[i].r = leds_last[i - 1].r*0.995;
      leds_temp[i].g = leds_last[i - 1].g*0.995;
      leds_temp[i].b = leds_last[i - 1].b*0.995;
    }
  }
  else{
    for (int16_t i = 127; i > 0; i--) {
      leds_temp[i].r = leds_last[i - 1].r;
      leds_temp[i].g = leds_last[i - 1].g;
      leds_temp[i].b = leds_last[i - 1].b;
    }
  }

  uint8_t final_val = 255*fft_sum;
  float color_val = hue-(30*fft_sum);

  leds_temp[0] = ColorFromPalette( current_palette, uint8_t(color_val), final_val, LINEARBLEND );
  //leds_temp[0] = CHSV(color_val, 255, final_val);
  
  load_leds_from_temp();
  save_leds_to_last();

  process_color_push();
}


void waveform_mode() {
  for (uint16_t i = 0; i < 128; i++) {
    uint16_t index_a = i * 2;
    uint16_t index_b = i * 2 + 1;

    if (index_b >= BUFFER_SIZE) {
      index_b = index_a;
    }

    int32_t sample_a = (i2s_samples[0][index_a] + i2s_samples[1][index_a] + i2s_samples[2][index_a] + i2s_samples[3][index_a] + i2s_samples[4][index_a] + i2s_samples[5][index_a]) / 6.0;
    int32_t sample_b = (i2s_samples[0][index_b] + i2s_samples[1][index_b] + i2s_samples[2][index_b] + i2s_samples[3][index_b] + i2s_samples[4][index_b] + i2s_samples[5][index_b]) / 6.0;
    int32_t sample = (sample_a + sample_b) >> 1;
    sample -= DC_OFFSET;
    sample *= 24;

    if (sample < -32767) {
      sample = -32767;
    }
    else if (sample > 32767) {
      sample = 32767;
    }

    sample += 32767;
    float val = sample / 65536.0;

    float out_val = val * 255;

    float final_val = (out_val * (0.75)) + (last_fft_frame[i] * (0.25));
    final_val = (final_val * (1.0 - SMOOTHING)) + (last_fft_frame[i] * (SMOOTHING));
    last_fft_frame[i] = final_val;

    final_val -= 64.0;
    if (final_val < 0.0) {
      final_val = 0.0;
    }
    final_val *= 2.0;

    if (final_val > 255) {
      final_val = 255;
    }

    final_val = uint8_t(uint16_t(final_val * final_val) >> 8); // gamma correction

    float color_val = hue - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.8) + (i * hue_shift_amount) - (32 * (final_val / 255.0));
    leds[i] = ColorFromPalette( current_palette, uint8_t(color_val), final_val, LINEARBLEND );
    //leds[i] = CHSV((hue) - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.8) + (i * hue_shift_amount) - (32 * (final_val / 255.0)), 255, final_val);
  }

  process_color_push();
}


void vu_mode() {
  static float fft_sum_last = 0.0;
  float fft_sum = 0.0;
  float max_push = 20.0;

  for (uint16_t i = 0; i < 128; i++) {
    fft_sum += processed_fft[i];
  }

  float out_val = (fft_sum * (1.0 - SMOOTHING)) + (fft_sum_last * (SMOOTHING));
  if (fabs(out_val - fft_sum_last) > max_push) {
    if (out_val > fft_sum_last) {
      out_val = fft_sum_last + max_push;
    }
    else if (out_val < fft_sum_last) {
      out_val = fft_sum_last - max_push;
    }
  }
  fft_sum_last = out_val;

  for (uint8_t i = 0; i < 128; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  float final_val = out_val * 2.0;
  if (final_val > 255) {
    final_val = 255;
  }
  for (uint8_t i = 0; i < uint8_t(out_val); i++) {
    float color_val = (hue) - uint8_t(uint16_t(final_val * final_val) >> 8) * (0.1 + (hue_push / 10.0) * 0.8) + (i * hue_shift_amount) - (32 * (final_val / 255.0));

    leds[i] = ColorFromPalette( current_palette, uint8_t(color_val), final_val, LINEARBLEND );
    //leds[i] = CHSV(color_val, 255, final_val);
  }
}

