CRGB calc_chromagram_color() {
  CRGB sum_color = CRGB(0, 0, 0);
  for (uint8_t i = 0; i < 12; i++) {
    float prog = i / 12.0;

    float bright = note_chromagram[i];
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER + 1; s++) {
      bright *= bright;
    }
    bright *= 0.5;

    if (bright > 1.0) {
      bright = 1.0;
    }

    if (chromatic_mode == true) {
      CRGB out_col = CHSV(255 * prog, 255 * CONFIG.SATURATION, 255 * bright);
      //out_col.r = scale8(out_col.r, out_col.r);
      //out_col.g = scale8(out_col.g, out_col.g);
      //out_col.b = scale8(out_col.b, out_col.b);
      sum_color += out_col;
    } else {
      sum_color += CHSV(255 * chroma_val + hue_shift, 255 * CONFIG.SATURATION, 255 * bright);
    }
  }

  if (chromatic_mode == false) {
    sum_color = force_saturation(sum_color, 255 * CONFIG.SATURATION);
  }

  return sum_color;
}

void avg_bins(uint8_t low_bin, uint8_t high_bin) {
  // TBD
}

// Default mode!
void light_mode_gdft() {
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {  // 64 freqs
    float bin = note_spectrogram_smooth[i] * 1.25;
    if (bin > 1.0) { bin = 1.0; }

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bin = (bin * bin);
    }

    bin = apply_contrast_float(bin, 0.10);

    bin *= 1.0 - CONFIG.BACKDROP_BRIGHTNESS;
    bin += CONFIG.BACKDROP_BRIGHTNESS;

    float led_brightness_raw = 254 * bin;  // -1 for temporal dithering below
    int16_t led_brightness = led_brightness_raw;
    float fract = led_brightness_raw - led_brightness;

    if (CONFIG.TEMPORAL_DITHERING == true) {
      if (fract >= dither_table[dither_step]) {
        led_brightness += 1;
      }
    }

    led_brightness -= 1;
    if (led_brightness < 0) {
      led_brightness = 0;
    }

    brightness_levels[i] = led_brightness;  // Can use this value later if needed

    //hue_shift += 0.01;

    float led_hue;
    if (chromatic_mode == true) {
      led_hue = 21.33333333 * i;  // Makes hue completely cycle once per octave
    } else {
      led_hue = 255 * chroma_val + (i >> 1) + hue_shift;  // User color selection
    }

    leds[i] = CHSV(led_hue - (bin * 32), 255 * CONFIG.SATURATION, brightness_levels[i]);
  }

  // Interpolate LEDs from resolution of 64 to 128
  scale_half_to_full(leds);
}

void light_mode_gdft_chromagram() {
  for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION);

    float bin = interpolate(prog, note_chromagram, 12) * 1.25;
    if (bin > 1.0) { bin = 1.0; };

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER + 1; s++) {
      bin = bin * bin;
    }

    bin *= 1.0 - CONFIG.BACKDROP_BRIGHTNESS;
    bin += CONFIG.BACKDROP_BRIGHTNESS;

    float led_brightness_raw = 254 * bin;  // -1 for temporal dithering below
    uint16_t led_brightness = led_brightness_raw;
    float fract = led_brightness_raw - led_brightness;

    if (CONFIG.TEMPORAL_DITHERING == true) {
      if (fract >= dither_table[dither_step]) {
        led_brightness += 1;
      }
    }

    float led_hue;
    if (chromatic_mode == true) {
      led_hue = 255 * prog;
    } else {
      led_hue = 255 * chroma_val + (i >> 1) + hue_shift;
    }

    leds[i] = CHSV(led_hue + hue_shift, 255 * CONFIG.SATURATION, led_brightness);
  }
}

void light_mode_bloom(bool fast_scroll) {
  static uint32_t iter = 0;
  const float led_share = 1.0 / 12.0;
  iter++;

  if (bitRead(iter, 0) == 0) {
    CRGB sum_color = calc_chromagram_color();

    //sum_color = force_saturation(sum_color, 255 * CONFIG.SATURATION);

    if (fast_scroll == true) {  // Fast mode scrolls two LEDs at a time
      for (uint8_t i = 0; i < NATIVE_RESOLUTION - 2; i++) {
        leds_fx[(NATIVE_RESOLUTION - 1) - i] = leds_last[(NATIVE_RESOLUTION - 1) - i - 2];
      }

      leds_fx[0] = sum_color;  // New information goes here
      leds_fx[1] = sum_color;  // New information goes here

    } else {  // Slow mode only scrolls one LED at a time
      for (uint8_t i = 0; i < NATIVE_RESOLUTION - 1; i++) {
        leds_fx[(NATIVE_RESOLUTION - 1) - i] = leds_last[(NATIVE_RESOLUTION - 1) - i - 1];
      }

      leds_fx[0] = sum_color;  // New information goes here
    }

    load_leds_from_fx();
    save_leds_to_last();

    //fadeToBlackBy(leds, 128, 255-255*waveform_peak_scaled);

    distort_logarithmic();
    //distort_exponential();

    fade_top_half(CONFIG.MIRROR_ENABLED);  // fade at different location depending if mirroring is enabled
    //increase_saturation(32);

    save_leds_to_aux();
  } else {
    load_leds_from_aux();
  }
}

void light_mode_vu() {
  const float led_share = 255 / float(12);
  //static float sum_color_last[3] = { 0, 0, 0 };

  float smoothing = (0.025 + CONFIG.MOOD * 0.975) * 0.25;
  float led_pos = waveform_peak_scaled * (NATIVE_RESOLUTION - 1);
  static float led_pos_smooth = 0.0;

  led_pos_smooth = led_pos * (smoothing) + led_pos_smooth * (1.0 - smoothing);

  if (led_pos_smooth > 126) {
    led_pos_smooth = 126;
  } else if (led_pos_smooth < 0) {
    led_pos_smooth = 0;
  }

  uint16_t led_pos_smooth_whole = led_pos_smooth;
  float fract = led_pos_smooth - led_pos_smooth_whole;

  CRGB sum_color = calc_chromagram_color();
  sum_color.maximizeBrightness();

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
    if (i < led_pos_smooth) {
      leds[i] = sum_color;
    } else if (i == led_pos_smooth) {
      leds[i] = CRGB(
        sum_color.r * fract,
        sum_color.g * fract,
        sum_color.b * fract);
    }
  }
}

void light_mode_vu_dot() {
  const float led_share = 255 / float(12);
  static float sum_color_last[3] = { 0, 0, 0 };
  static float led_pos_last = 0;

  float smoothing = (0.025 + CONFIG.MOOD * 0.975) * 0.25;
  float led_pos = waveform_peak_scaled * (NATIVE_RESOLUTION - 1);
  static float led_pos_smooth = 0.0;

  led_pos_smooth = led_pos * (smoothing) + led_pos_smooth * (1.0 - smoothing);

  if (led_pos_smooth > NATIVE_RESOLUTION - 2) {
    led_pos_smooth = NATIVE_RESOLUTION - 2;
  } else if (led_pos_smooth < 0) {
    led_pos_smooth = 0;
  }

  CRGB sum_color = calc_chromagram_color();

  //if (sum_color.r < 5) { sum_color.r = 5; }
  //if (sum_color.g < 5) { sum_color.g = 5; }
  //if (sum_color.b < 5) { sum_color.b = 5; }

  fadeToBlackBy(leds, NATIVE_RESOLUTION, 255);

  if (led_pos_last < led_pos_smooth) {
    for (uint8_t i = led_pos_last; i <= led_pos_smooth; i++) {
      leds[i] = sum_color;
      leds[i + 1] = sum_color;
    }
  } else if (led_pos_last > led_pos_smooth) {
    for (uint8_t i = led_pos_smooth; i <= led_pos_last; i++) {
      leds[i] = sum_color;
      leds[i + 1] = sum_color;
    }
  } else {
    leds[uint8_t(led_pos_smooth)] = sum_color;
    leds[uint8_t(led_pos_smooth) + 1] = sum_color;
  }

  led_pos_last = led_pos_smooth;

  save_leds_to_fx();
  scale_image_to_half(leds_fx);

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    CRGB col = leds_fx[i];

    leds_fx[i] = CRGB(
      col.g,
      col.b,
      col.r);
  }

  save_leds_to_temp();
  blend_buffers(leds, leds_temp, leds_fx, BLEND_ADD, 32);

  save_leds_to_fx();
  scale_half_to_full(leds_fx);

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    CRGB col = leds_fx[i];

    leds_fx[i] = CRGB(
      col.g,
      col.b,
      col.r);
  }

  save_leds_to_temp();
  blend_buffers(leds, leds_temp, leds_fx, BLEND_ADD, 32);
}

void light_mode_kaleidoscope() {
  float kaleidoscope_mood = (CONFIG.MOOD * 0.75 + 0.25) * 0.09;

  static float pos_r = 0.0;
  static float pos_g = 1000.0;
  static float pos_b = 10000.0;

  static float punch_r_decay = 0.0;
  static float punch_g_decay = 0.0;
  static float punch_b_decay = 0.0;

  uint8_t sat = 255 * CONFIG.SATURATION;  // Do this float math once ahead of time

  // BEGIN FIXED-POINT-ONLY ZONE ----------------------------------------
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    uint32_t x_pos = i << 12;
    uint32_t y_pos_r = uint32_t(pos_r);
    uint32_t y_pos_g = uint32_t(pos_g);
    uint32_t y_pos_b = uint32_t(pos_b);

    uint8_t r_val = inoise16(x_pos, y_pos_r) >> 8;
    uint8_t g_val = inoise16(x_pos, y_pos_g) >> 8;
    uint8_t b_val = inoise16(x_pos, y_pos_b) >> 8;

    for (uint8_t i = 0; i < CONFIG.SQUARE_ITER + 1; i++) {
      r_val = uint16_t(r_val * r_val) >> 8;
      g_val = uint16_t(g_val * g_val) >> 8;
      b_val = uint16_t(b_val * b_val) >> 8;
    }

    //r_val = apply_contrast(r_val, 16);
    //g_val = apply_contrast(g_val, 16);
    //b_val = apply_contrast(b_val, 16);

    if (chromatic_mode == true) {
      leds[i] = force_saturation(CRGB(r_val, g_val, b_val), sat);
    } else {
      leds[i] = force_saturation(CRGB(r_val, g_val, b_val), 0);

      uint8_t chroma = 255 * chroma_val;

      CRGB base_hue = CHSV(chroma + (i >> 1) + hue_shift, sat, 255);

      leds[i].r = uint16_t(leds[i].r * base_hue.r) >> 8;
      leds[i].g = uint16_t(leds[i].g * base_hue.g) >> 8;
      leds[i].b = uint16_t(leds[i].b * base_hue.b) >> 8;
    }
  }

  // END FIXED-POINT-ONLY ZONE ------------------------------------------

  punch_r_decay *= 0.8;
  punch_g_decay *= 0.8;
  punch_b_decay *= 0.8;

  //float punch_r = avg_bins(0, 20) * kaleidoscope_mood;

  float punch_r = calc_punch(0, 20) * kaleidoscope_mood;
  float punch_g = calc_punch(20, 40) * kaleidoscope_mood;
  float punch_b = calc_punch(40, 60) * kaleidoscope_mood;

  if (punch_r >= punch_r_decay) { punch_r_decay = punch_r; }
  if (punch_g >= punch_g_decay) { punch_g_decay = punch_g; }
  if (punch_b >= punch_b_decay) { punch_b_decay = punch_b; }

  pos_r += (((0.25 * kaleidoscope_mood) + punch_r_decay)) * 16384;
  pos_g += (((0.26 * kaleidoscope_mood) + punch_g_decay)) * 16384;
  pos_b += (((0.27 * kaleidoscope_mood) + punch_b_decay)) * 16384;
}