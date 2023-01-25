// Default mode!
void light_mode_gdft() {
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {  // 64 freqs
    float bin = note_spectrogram_smooth[i];
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bin = bin * bin;
    }
    uint16_t led_brightness = 255 * bin;

    brightness_levels[i] = led_brightness;  // Can use this value later if needed

    float led_hue;
    if (chromatic_mode == true) {
      led_hue = 21.33333333 * i;  // Makes hue completely cycle once per octave
    }
    else {
      led_hue = 255 * chroma_val;  // User color selection
    }

    CRGB col1 = CRGB(0, 0, 0);
    hsv2rgb_spectrum(  // Spectrum has better low-light color resolution than default "rainbow" HSV behavior
      CHSV(led_hue, 255, brightness_levels[i]),
      col1);

    CRGB col2 = CRGB(0, 0, 0);
    hsv2rgb_spectrum(  // Spectrum has better low-light color resolution than default "rainbow" HSV behavior
      CHSV(led_hue + 10.66666666, 255, brightness_levels[i]),
      col2);

    leds[i * 2 + 0] = col1;  // Two LEDs at a time so that mirror mode works gracefully
    leds[i * 2 + 1] = col2;
  }
}

void light_mode_gdft_chromagram() {
  for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION);

    float bin = interpolate(prog, note_chromagram, 12);

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bin = bin * bin;
    }

    float led_hue;
    if (chromatic_mode == true) {
      led_hue = 255 * prog;
    }
    else {
      led_hue = 255 * chroma_val;
    }

    leds[i] = CHSV(led_hue, 255, 255 * bin);
  }
}

void light_mode_bloom(bool fast_scroll) {
  static uint32_t iter = 0;
  const float led_share = 255 / float(12);

  iter++;

  CRGB sum_color = CRGB(0, 0, 0);
  float brightness_sum = 0.0;
  for (uint8_t i = 0; i < 12; i++) {
    float prog = i / float(12);

    float bin = note_chromagram[i]; // * (1.0 / chromagram_max_val);

    float bright = bin;
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bright *= bright;
    }
    bright *= 1.5;
    if (bright > 1.0) {
      bright = 1.0;
    }

    bright *= led_share;

    CRGB out_col;
    if (chromatic_mode == true) {
      hsv2rgb_spectrum(
        CHSV(255 * prog, 255, bright),
        out_col);
    }
    else {
      brightness_sum += bright;
    }

    sum_color += out_col;
  }

  if (chromatic_mode == false) {
    hsv2rgb_spectrum(
      CHSV(255 * chroma_val, 255, brightness_sum),
      sum_color);
  }

  if (fast_scroll == true) { // Fast mode scrolls two LEDs at a time
    for (uint8_t i = 0; i < NATIVE_RESOLUTION - 2; i++) {
      leds_temp[(NATIVE_RESOLUTION - 1) - i] = leds_last[(NATIVE_RESOLUTION - 1) - i - 2];
    }

    leds_temp[0] = sum_color; // New information goes here
    leds_temp[1] = sum_color; // New information goes here
  }
  else { // Slow mode only scrolls one LED at a time
    for (uint8_t i = 0; i < NATIVE_RESOLUTION - 1; i++) {
      leds_temp[(NATIVE_RESOLUTION - 1) - i] = leds_last[(NATIVE_RESOLUTION - 1) - i - 1];
    }

    leds_temp[0] = sum_color; // New information goes here
  }

  load_leds_from_temp();
  save_leds_to_last();

  //fadeToBlackBy(leds, 128, 255-255*waveform_peak_scaled);

  distort_logarithmic();
  //distort_exponential();

  fade_top_half(CONFIG.MIRROR_ENABLED); // fade at different location depending if mirroring is enabled
  increase_saturation(32);
}

void light_mode_waveform() {
  const float led_share = 255 / float(12);
  static float waveform_peak_scaled_last;
  static float waveform_last[1024] = { 0 };
  static float sum_color_last[3] = {0, 0, 0};

  waveform_peak_scaled_last = (waveform_peak_scaled * 0.05 + waveform_peak_scaled_last * 0.95);

  CRGB sum_color = CRGB(0, 0, 0);
  float brightness_sum = 0.0;
  for (uint8_t c = 0; c < 12; c++) {
    float prog = c / float(12);
    float bin = note_chromagram[c] * (1.0 / chromagram_max_val);

    float bright = bin;
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bright *= bright;
    }
    bright *= 1.5;
    if (bright > 1.0) {
      bright = 1.0;
    }

    bright *= led_share;

    if (chromatic_mode == true) {
      CRGB out_col;
      hsv2rgb_spectrum(
        CHSV(255 * prog, 255, bright),
        out_col);

      sum_color += out_col;
    }
    else {
      brightness_sum += bright;
    }
  }

  if (chromatic_mode == false) {
    hsv2rgb_spectrum(
      CHSV(255 * chroma_val, 255, brightness_sum),
      sum_color);
  }

  /*
  CHSV hsv = rgb2hsv_approximate(sum_color);
  hsv.s = qadd8(hsv.s, 64);
  sum_color = hsv;
  */

  float sum_color_float[3] = {float(sum_color.r), float(sum_color.g), float(sum_color.b)};

  sum_color_float[0] = sum_color_float[0] * 0.05 + sum_color_last[0] * 0.95;
  sum_color_float[1] = sum_color_float[1] * 0.05 + sum_color_last[1] * 0.95;
  sum_color_float[2] = sum_color_float[2] * 0.05 + sum_color_last[2] * 0.95;

  sum_color_last[0] = sum_color_float[0];
  sum_color_last[1] = sum_color_float[1];
  sum_color_last[2] = sum_color_float[2];

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float waveform_sample = 0.0;
    for (uint8_t s = 0; s < 4; s++) {
      waveform_sample += waveform_history[s][i];
    }
    waveform_sample /= 4.0;
    float input_wave_sample = (waveform_sample / 128.0);

    //----------------------

    float smoothing = (0.1 + CONFIG.MOOD * 0.9) * 0.05;

    waveform_last[i] = input_wave_sample * (smoothing) + waveform_last[i] * (1.0 - smoothing);

    float peak = waveform_peak_scaled_last * 4.0;
    if (peak > 1.0) {
      peak = 1.0;
    }

    float output_brightness = (waveform_last[i]);
    if (output_brightness > 1.0) {
      output_brightness = 1.0;
    }

    output_brightness = 0.5 + output_brightness * 0.5;
    if (output_brightness > 1.0) {
      output_brightness = 1.0;
    }
    else if (output_brightness < 0.0) {
      output_brightness = 0.0;
    }

    output_brightness *= peak;

    leds[i] = CRGB(
                sum_color_float[0] * output_brightness,
                sum_color_float[1] * output_brightness,
                sum_color_float[2] * output_brightness
              );
  }
}

void light_mode_vu() {
  const float led_share = 255 / float(12);
  static float sum_color_last[3] = {0, 0, 0};

  float smoothing = (0.1 + CONFIG.MOOD * 0.9) * 0.25;
  float led_pos = waveform_peak_scaled * (NATIVE_RESOLUTION - 1);
  static float led_pos_smooth = 0.0;

  led_pos_smooth = led_pos * (smoothing) + led_pos_smooth * (1.0 - smoothing);

  if (led_pos_smooth > 126) {
    led_pos_smooth = 126;
  }
  else if (led_pos_smooth < 0) {
    led_pos_smooth = 0;
  }

  CRGB sum_color = CRGB(0, 0, 0);
  if (chromatic_mode == true) {
    for (uint8_t c = 0; c < 12; c++) {
      float prog = c / float(12);
      float bin = note_chromagram[c] * (1.0 / chromagram_max_val);

      CRGB out_col;
      hsv2rgb_spectrum(
        CHSV(255 * prog, 255, led_share * bin),
        out_col);

      sum_color += out_col;
    }
  }
  else {
    sum_color = CHSV(255 * chroma_val, 255, 255); // User color selection
  }

  CHSV hsv = rgb2hsv_approximate(sum_color);
  hsv.s = qadd8(hsv.s, 64);
  hsv.v = qadd8(hsv.v, 64);
  sum_color = hsv;

  float sum_color_float[3] = {float(sum_color.r), float(sum_color.g), float(sum_color.b)};

  sum_color_float[0] = sum_color_float[0] * 0.05 + sum_color_last[0] * 0.95;
  sum_color_float[1] = sum_color_float[1] * 0.05 + sum_color_last[1] * 0.95;
  sum_color_float[2] = sum_color_float[2] * 0.05 + sum_color_last[2] * 0.95;

  sum_color_last[0] = sum_color_float[0];
  sum_color_last[1] = sum_color_float[1];
  sum_color_last[2] = sum_color_float[2];

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
    if (i <= led_pos_smooth) {
      leds[i] = CRGB(
                  sum_color_float[0],
                  sum_color_float[1],
                  sum_color_float[2]
                );
    }
  }
}

void light_mode_vu_dot() {
  const float led_share = 255 / float(12);
  static float sum_color_last[3] = {0, 0, 0};
  static float led_pos_last = 0;

  float smoothing = (0.1 + CONFIG.MOOD * 0.9) * 0.25;
  float led_pos = waveform_peak_scaled * (NATIVE_RESOLUTION - 1);
  static float led_pos_smooth = 0.0;

  led_pos_smooth = led_pos * (smoothing) + led_pos_smooth * (1.0 - smoothing);

  if (led_pos_smooth > 126) {
    led_pos_smooth = 126;
  }
  else if (led_pos_smooth < 0) {
    led_pos_smooth = 0;
  }

  CRGB sum_color = CRGB(0, 0, 0);
  if (chromatic_mode == true) {
    for (uint8_t c = 0; c < 12; c++) {
      float prog = c / float(12);
      float bin = note_chromagram[c] * (1.0 / chromagram_max_val);

      CRGB out_col;
      hsv2rgb_spectrum(
        CHSV(255 * prog, 255, led_share * bin),
        out_col);

      sum_color += out_col;
    }
  }
  else {
    sum_color = CHSV(255 * chroma_val, 255, 255); // User color selection
  }

  CHSV hsv = rgb2hsv_approximate(sum_color);
  hsv.s = qadd8(hsv.s, 64);
  hsv.v = qadd8(hsv.v, 64);
  sum_color = hsv;

  float sum_color_float[3] = {float(sum_color.r), float(sum_color.g), float(sum_color.b)};

  sum_color_float[0] = sum_color_float[0] * 0.05 + sum_color_last[0] * 0.95;
  sum_color_float[1] = sum_color_float[1] * 0.05 + sum_color_last[1] * 0.95;
  sum_color_float[2] = sum_color_float[2] * 0.05 + sum_color_last[2] * 0.95;

  sum_color_last[0] = sum_color_float[0];
  sum_color_last[1] = sum_color_float[1];
  sum_color_last[2] = sum_color_float[2];

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  if (led_pos_last < led_pos_smooth) {
    for (uint8_t i = led_pos_last; i <= led_pos_smooth; i++) {
      leds[i] = CRGB(
                  sum_color_float[0],
                  sum_color_float[1],
                  sum_color_float[2]
                );
      leds[i + 1] = CRGB(
                      sum_color_float[0],
                      sum_color_float[1],
                      sum_color_float[2]
                    );
    }
  }
  else if (led_pos_last > led_pos_smooth) {
    for (uint8_t i = led_pos_smooth; i <= led_pos_last; i++) {
      leds[i] = CRGB(
                  sum_color_float[0],
                  sum_color_float[1],
                  sum_color_float[2]
                );
      leds[i + 1] = CRGB(
                      sum_color_float[0],
                      sum_color_float[1],
                      sum_color_float[2]
                    );
    }
  }
  else {
    leds[uint8_t(led_pos_smooth)] = CRGB(
                                      sum_color_float[0],
                                      sum_color_float[1],
                                      sum_color_float[2]
                                    );
    leds[uint8_t(led_pos_smooth) + 1] = CRGB(
                                          sum_color_float[0],
                                          sum_color_float[1],
                                          sum_color_float[2]
                                        );
  }

  led_pos_last = led_pos_smooth;
}
