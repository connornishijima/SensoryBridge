/*----------------------------------------
  Sensory Bridge LIGHTSHOW FUNCTIONS
----------------------------------------*/

void duet_mode() {
  for (uint8_t i = 0; i < 128; i++) {
    float fft_val = fft_processed[i];

    if (fft_val < 0.0) {
      fft_val = 0.0;
    }
    if (fft_val > 1.0) {
      fft_val = 1.0;
    }

    uint16_t final_level = 255 * (fft_val * fft_val);

    if (final_level > 255) {
      final_level = 255;
    }

    hsv2rgb_spectrum(
      CHSV(
        CONFIG.BASE_HUE + (hue_scatter[i]) - (fft_val * 32) - (64 * (i / 128.0)),
        255,
        final_level),
      leds[i]);
  }
}

void bloom_mode(bool fast_mode) {
  float fft_sum = 0.0;
  for (uint8_t i = 0; i < 128; i++) {
    fft_sum += fft_output_history[fft_output_history_index][i];
  }
  fft_sum /= 96.0;

  if (fft_sum > 1.0) {
    fft_sum = 1.0;
  }

  fft_sum = fft_sum * fft_sum;

  uint8_t base_index = 0;
  if (fast_mode) {
    base_index = 1;
  }

  for (int16_t i = 127; i > base_index; i--) {
    leds_temp[i].r = leds_last[i - (1 + base_index)].r;
    leds_temp[i].g = leds_last[i - (1 + base_index)].g;
    leds_temp[i].b = leds_last[i - (1 + base_index)].b;
  }

  hsv2rgb_spectrum(
    CHSV(CONFIG.BASE_HUE - (30 * fft_sum), 255, 255 * fft_sum),
    leds_temp[0]);

  if (fast_mode) {
    hsv2rgb_spectrum(
      CHSV(CONFIG.BASE_HUE - (30 * fft_sum), 255, 255 * fft_sum),
      leds_temp[1]);
  }

  load_leds_from_temp();
  save_leds_to_last();
}

void waveform_mode() {
  for (uint16_t i = 0; i < 128; i++) {
    uint16_t index_a = i * 2;
    uint16_t index_b = i * 2 + 1;

    if (index_b >= BUFFER_SIZE) {
      index_b = index_a;
    }

    int32_t sample_a = (i2s_samples[0][index_a] + i2s_samples[1][index_a] + i2s_samples[2][index_a] + i2s_samples[3][index_a]) >> 2;
    int32_t sample_b = (i2s_samples[0][index_b] + i2s_samples[1][index_b] + i2s_samples[2][index_b] + i2s_samples[3][index_b]) >> 2;
    int32_t sample = (sample_a + sample_b) >> 1;
    sample -= CONFIG.DC_OFFSET;
    sample *= 48;

    if (sample < -32767) {
      sample = -32767;
    } else if (sample > 32767) {
      sample = 32767;
    }

    sample += 16384;

    if (sample < 0) {
      sample = 0;
    }

    sample *= 2;

    if (sample > 65535) {
      sample = 65535;
    }

    float val = sample / 65536.0;

    uint8_t out_val = val * val * 255;

    hsv2rgb_spectrum(
      CHSV(
        CONFIG.BASE_HUE + (hue_scatter[i]) - (64 * (i / 128.0)),
        255,
        out_val),
      leds[i]);

    float smooth_low = 0.05;
    float smooth_high = 0.3;

    float smooth_factor = (CONFIG.MOOD) * (smooth_high - smooth_low) / (1.0) + smooth_low;

    leds[i].r = (leds_last[i].r * (1.0 - smooth_factor)) + (leds[i].r * smooth_factor);
    leds[i].g = (leds_last[i].g * (1.0 - smooth_factor)) + (leds[i].g * smooth_factor);
    leds[i].b = (leds_last[i].b * (1.0 - smooth_factor)) + (leds[i].b * smooth_factor);
  }

  save_leds_to_last();
}

void vu_mode(bool dot_mode) {
  static int32_t max_val_cap = -10000000;
  static int32_t min_val_cap = 10000000;
  static int32_t short_term_delta_last = 1;

  int32_t max_val = -10000000;
  int32_t min_val = 10000000;
  for (uint16_t i = 0; i < BUFFER_SIZE; i++) {
    int32_t audio_sample = ((i2s_samples[0][i] + i2s_samples[1][i] + i2s_samples[2][i] + i2s_samples[3][i]) >> 2) - CONFIG.DC_OFFSET;
    if (audio_sample < min_val_cap) {
      min_val_cap = audio_sample;
    }
    if (audio_sample > max_val_cap) {
      max_val_cap = audio_sample;
    }

    if (audio_sample < min_val) {
      min_val = audio_sample;
    }
    if (audio_sample > max_val) {
      max_val = audio_sample;
    }
  }

  if (max_val_cap < 700) {
    max_val_cap = 700;
  }
  if (min_val_cap > -700) {
    min_val_cap = -700;
  }

  int32_t short_term_delta = abs(max_val - min_val);
  int32_t long_term_delta = abs(max_val_cap - min_val_cap);

  max_val_cap *= 0.99;
  min_val_cap *= 0.99;

  float smooth_factor = 0.05 + (CONFIG.MOOD * 0.45);
  short_term_delta = (short_term_delta * (smooth_factor)) + (short_term_delta_last * (1.0 - smooth_factor));
  short_term_delta_last = short_term_delta;

  float audio_level = short_term_delta / float(long_term_delta);
  if (audio_level > 1.0) {
    audio_level = 1.0;
  }

  audio_level *= 126.0;

  float audio_level_out = 0.0;
  memcpy(&audio_level_out, &audio_level, sizeof(float));

  int audio_level_floored = floor(audio_level_out);
  if (audio_level_floored < 0) {
    audio_level_floored = 0;
  } else if (audio_level_floored > 127) {
    audio_level_floored = 127;
  }

  float audio_level_lerp = audio_level_out - audio_level_floored;

  for (uint8_t i = 0; i < 128; i++) {
    leds[i].r = 0;//leds[i].r * 0.5;
    leds[i].g = 0;//leds[i].g * 0.5;
    leds[i].b = 0;//leds[i].b * 0.5;
  }

  if (dot_mode == false) {  // Standard display
    for (uint8_t i = 0; i < 128; i++) {
      float output_level;

      if (i <= audio_level_floored) {
        output_level = 1.0;
      } else if (i == audio_level_floored + 1) {
        output_level = audio_level_lerp;
      } else {
        output_level = 0.0;
      }

      hsv2rgb_spectrum(
        CHSV(
          CONFIG.BASE_HUE + (hue_scatter[i]) - (64 * (i / 128.0)),
          255,
          255 * output_level),
        leds[i]);
    }
  } else if (dot_mode == true) {  // Dot mode
    float dot_pos = audio_level_floored;
    float dot_lerp = audio_level_lerp;
    static float dot_pos_last = 0;

    float dot_delta = fabs(dot_pos - dot_pos_last);
    float output_level = 8.0;
    output_level /= dot_delta;
    if (output_level > 1.0) {
      output_level = 1.0;
    }

    int16_t wipe_start = dot_pos_last - 1;
    int16_t wipe_end = dot_pos + 1;
    if (dot_pos < dot_pos_last) {
      wipe_start = dot_pos - 1;
      wipe_end = dot_pos_last + 1;
    }

    if (wipe_start < 1) {
      wipe_start = 1;
    }
    if (wipe_end > 126) {
      wipe_end = 126;
    }

    for (int16_t i = wipe_start; i < wipe_end; i++) {
      leds[i] += CHSV(
        CONFIG.BASE_HUE + (hue_scatter[i]) - (64 * (i / 128.0)),
        255,
        255 * (output_level * output_level));
    }

    float lerp_level;
    lerp_level = (1.0 - dot_lerp) * output_level;
    leds[wipe_start - 1] += CHSV(
      CONFIG.BASE_HUE + (hue_scatter[wipe_start - 1]) - (64 * ((wipe_start - 1) / 128.0)),
      255,
      255 * (lerp_level * lerp_level));

    lerp_level = dot_lerp * output_level;
    leds[wipe_end] += CHSV(
      CONFIG.BASE_HUE + (hue_scatter[wipe_end]) - (64 * ((wipe_end) / 128.0)),
      255,
      255 * (lerp_level * lerp_level));

    dot_pos_last = dot_pos;
  }
}