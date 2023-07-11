void get_smooth_spectrogram() {
  static SQ15x16 spectrogram_smooth_last[64];

  for (uint8_t bin = 0; bin < 64; bin++) {
    SQ15x16 note_brightness = spectrogram[bin];

    if (spectrogram_smooth[bin] < note_brightness) {
      SQ15x16 distance = note_brightness - spectrogram_smooth[bin];
      spectrogram_smooth[bin] += distance * SQ15x16(0.75);

    } else if (spectrogram_smooth[bin] > note_brightness) {
      SQ15x16 distance = spectrogram_smooth[bin] - note_brightness;
      spectrogram_smooth[bin] -= distance * SQ15x16(0.75);
    }
  }
}

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
      //sum_color += hsv(255 * float(chroma_val) + float(hue_shift), 255 * CONFIG.SATURATION, 255 * bright);
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

void test_mode() {
  static float radians = 0.00;
  radians += CONFIG.MOOD;
  float position = sin(radians) * 0.5 + 0.5;
  set_dot_position(RESERVED_DOTS + 0, position);
  clear_leds();
  draw_dot(leds_16, RESERVED_DOTS + 0, hsv(chroma_val, CONFIG.SATURATION, CONFIG.PHOTONS * CONFIG.PHOTONS));
}

// Default mode!
void light_mode_gdft() {
  for (SQ15x16 i = 0; i < NUM_FREQS; i += 1) {  // 64 freqs
    SQ15x16 prog = i / (SQ15x16)NUM_FREQS;
    SQ15x16 bin = spectrogram_smooth[i.getInteger()];
    if (bin > 1.0) { bin = 1.0; }

    uint8_t extra_iters = 0;
    if (chromatic_mode == true) {
      extra_iters = 1;
    }
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER + extra_iters; s++) {
      bin = (bin * bin) * SQ15x16(0.65) + (bin * SQ15x16(0.35));
    }

    //bin = apply_contrast_fixed(bin, 0.04);

    //bin *= SQ15x16(1.0 - CONFIG.BACKDROP_BRIGHTNESS);
    //bin += SQ15x16(CONFIG.BACKDROP_BRIGHTNESS);

    SQ15x16 led_hue;
    if (chromatic_mode == true) {
      led_hue = note_colors[i.getInteger() % 12];  // Makes hue completely cycle once per octave
    } else {
      led_hue = chroma_val + hue_position + ((sqrt(float(bin)) * SQ15x16(0.05)) + (prog * SQ15x16(0.10)) * hue_shifting_mix);
    }

    leds_16[i.getInteger()] = hsv(led_hue + bin * SQ15x16(0.050), CONFIG.SATURATION, bin);
  }

  shift_leds_up(leds_16, 64);       // (led_utilities.h) Move image up one half
  mirror_image_downwards(leds_16);  // (led_utilities.h) Mirror downwards
}

/*
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
      //led_hue = 255 * prog;
    } else {
      //led_hue = 255 * chroma_val + (i >> 1) + hue_shift;
    }

    //leds[i] = CHSV(led_hue + hue_shift, 255 * CONFIG.SATURATION, led_brightness);
  }
}
*/

/*
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
*/

void light_mode_vu_dot() {
  static SQ15x16 dot_pos_last = 0.0;
  static SQ15x16 audio_vu_level_smooth = 0.0;
  static SQ15x16 max_level = 0.01;

  SQ15x16 mix_amount = mood_scale(0.10, 0.05);

  audio_vu_level_smooth = (audio_vu_level_average * mix_amount) + (audio_vu_level_smooth * (1.0 - mix_amount));

  if (audio_vu_level_smooth * 1.1 > max_level) {
    SQ15x16 distance = (audio_vu_level_smooth * 1.1) - max_level;
    max_level += distance *= 0.1;
  } else {
    max_level *= 0.9999;
    if (max_level < 0.0025) {
      max_level = 0.0025;
    }
  }
  SQ15x16 multiplier = 1.0 / max_level;

  SQ15x16 dot_pos = (audio_vu_level_smooth * multiplier);

  if (dot_pos > 1.0) {
    dot_pos = 1.0;
  }

  SQ15x16 mix = mood_scale(0.25, 0.24);
  SQ15x16 dot_pos_smooth = (dot_pos * mix) + (dot_pos_last * (1.0-mix));
  dot_pos_last = dot_pos_smooth;

  SQ15x16 brightness = sqrt(float(dot_pos_smooth));

  set_dot_position(RESERVED_DOTS + 0, dot_pos_smooth * 0.5 + 0.5);
  set_dot_position(RESERVED_DOTS + 1, 0.5 - dot_pos_smooth * 0.5);

  clear_leds();
  //fade_grayscale(0.15);

  SQ15x16 hue = chroma_val + hue_position;
  CRGB16 color = hsv(hue, CONFIG.SATURATION, brightness);
  draw_dot(leds_16, RESERVED_DOTS + 0, color);
  draw_dot(leds_16, RESERVED_DOTS + 1, color);
}

void light_mode_kaleidoscope() {
  static float pos_r = 0.0;
  static float pos_g = 0.0;
  static float pos_b = 0.0;

  static SQ15x16 brightness_low = 0.0;
  static SQ15x16 brightness_mid = 0.0;
  static SQ15x16 brightness_high = 0.0;





  SQ15x16 sum_low = 0.0;
  SQ15x16 sum_mid = 0.0;
  SQ15x16 sum_high = 0.0;

  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[0 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    sum_low += bin;
    if (bin > brightness_low) {
      SQ15x16 dist = fabs_fixed(bin - brightness_low);
      brightness_low += dist * 0.1;
    }
  }
  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[20 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    sum_mid += bin;
    if (bin > brightness_mid) {
      SQ15x16 dist = fabs_fixed(bin - brightness_mid);
      brightness_mid += dist * 0.1;
    }
  }
  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[40 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    sum_high += bin;
    if (bin > brightness_high) {
      SQ15x16 dist = fabs_fixed(bin - brightness_high);
      brightness_high += dist * 0.1;
    }
  }

  brightness_low *= 0.99;
  brightness_mid *= 0.99;
  brightness_high *= 0.99;

  SQ15x16 shift_speed = (SQ15x16)100 + ((SQ15x16)500 * (SQ15x16)CONFIG.MOOD);

  SQ15x16 shift_r = (shift_speed * sum_low);
  SQ15x16 shift_g = (shift_speed * sum_mid);
  SQ15x16 shift_b = (shift_speed * sum_high);

  SQ15x16 speed_limit = (SQ15x16)2000 + (SQ15x16)2000 * (SQ15x16)CONFIG.MOOD;

  pos_r += (float)shift_r;
  pos_g += (float)shift_g;
  pos_b += (float)shift_b;





  for (uint8_t i = 0; i < 64; i++) {
    uint32_t y_pos_r = pos_r;
    uint32_t y_pos_g = pos_g;
    uint32_t y_pos_b = pos_b;

    uint32_t i_shifted = i + 18;
    uint32_t i_scaled = (i_shifted * i_shifted * i_shifted);

    SQ15x16 r_val = inoise16(i_scaled * 0.5 + y_pos_r) / 65536.0;
    SQ15x16 g_val = inoise16(i_scaled * 1.0 + y_pos_g) / 65536.0;
    SQ15x16 b_val = inoise16(i_scaled * 1.5 + y_pos_b) / 65536.0;

    if (r_val > 1.0) { r_val = 1.0; };
    if (g_val > 1.0) { g_val = 1.0; };
    if (b_val > 1.0) { b_val = 1.0; };

    for (uint8_t i = 0; i < CONFIG.SQUARE_ITER + 1; i++) {
      r_val *= r_val;
      g_val *= g_val;
      b_val *= b_val;
    }

    r_val = apply_contrast_fixed(r_val, 0.1);
    g_val = apply_contrast_fixed(g_val, 0.1);
    b_val = apply_contrast_fixed(b_val, 0.1);

    SQ15x16 prog = 1.0;
    if (i < 32) {
      prog = (i / 31.0);
    }
    prog *= prog;

    r_val *= prog * brightness_low;
    g_val *= prog * brightness_mid;
    b_val *= prog * brightness_high;

    CRGB16 col = { r_val, g_val, b_val };
    col = desaturate(col, 0.1 + (0.9 - 0.9*CONFIG.SATURATION));

    if (chromatic_mode == false) {
      SQ15x16 brightness = 0.0;
      if(r_val > brightness){ brightness = r_val; }
      if(g_val > brightness){ brightness = g_val; }
      if(b_val > brightness){ brightness = b_val; }

      SQ15x16 led_hue = chroma_val + hue_position + ((sqrt(float(brightness)) * SQ15x16(0.05)) + (prog * SQ15x16(0.10)) * hue_shifting_mix);
      col = hsv(led_hue, CONFIG.SATURATION, brightness);
    }

    leds_16[i] = { col.r, col.g, col.b };
    leds_16[NATIVE_RESOLUTION - 1 - i] = leds_16[i];
  }
}

void light_mode_chromagram_gradient() {
  for (uint8_t i = 0; i < 64; i++) {
    SQ15x16 prog = i / 64.0;
    SQ15x16 note_magnitude = interpolate(prog, chromagram_smooth, 12) * 0.9 + 0.1;

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      note_magnitude = (note_magnitude * note_magnitude) * SQ15x16(0.65) + (note_magnitude * SQ15x16(0.35));
    }

    SQ15x16 led_hue;
    if (chromatic_mode == true) {
      led_hue = interpolate(prog, note_colors, 12);
    } else {
      led_hue = chroma_val + hue_position + ((sqrt(float(note_magnitude)) * SQ15x16(0.05)) + (prog * SQ15x16(0.10)) * hue_shifting_mix);
    }

    CRGB16 col = hsv(led_hue, CONFIG.SATURATION, note_magnitude * note_magnitude);

    leds_16[64 + i] = col;
    leds_16[63 - i] = col;
  }
}

void light_mode_chromagram_dots() {
  static SQ15x16 chromagram_last[12];

  memset(leds_16, 0, sizeof(CRGB16) * 128);
  //dim_display(0.9);

  low_pass_array_fixed(chromagram_smooth, chromagram_last, 12, LED_FPS, float(mood_scale(3.5, 1.5)));
  memcpy(chromagram_last, chromagram_smooth, sizeof(float) * 12);

  for (uint8_t i = 0; i < 12; i++) {
    SQ15x16 led_hue;
    if (chromatic_mode == true) {
      led_hue = note_colors[i];
    } else {
      led_hue = chroma_val + hue_position + (sqrt(float(1.0)) * SQ15x16(0.05));
    }

    SQ15x16 magnitude = chromagram_smooth[i] * 1.0;
    if (magnitude > 1.0) { magnitude = 1.0; }

    magnitude = magnitude * magnitude;

    CRGB16 col = hsv(led_hue, CONFIG.SATURATION, magnitude);

    set_dot_position(RESERVED_DOTS + i * 2 + 0, magnitude * 0.45 + 0.5);
    set_dot_position(RESERVED_DOTS + i * 2 + 1, 0.5 - magnitude * 0.45);

    draw_dot(leds_16, RESERVED_DOTS + i * 2 + 0, col);
    draw_dot(leds_16, RESERVED_DOTS + i * 2 + 1, col);
  }
}

void light_mode_bloom() {
  // Clear output
  memset(leds_16, 0, sizeof(CRGB16) * NATIVE_RESOLUTION);

  draw_sprite(leds_16, leds_16_prev, 128, 128, 0.250 + 1.750 * CONFIG.MOOD, 0.99);

  //-------------------------------------------------------

  /*
  SQ15x16 brightness_low = 0.0;
  SQ15x16 brightness_mid = 0.0;
  SQ15x16 brightness_high = 0.0;

  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[0 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    brightness_low += bin;
  }
  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[20 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    brightness_mid += bin;
  }
  for (uint8_t i = 0; i < 20; i++) {
    SQ15x16 bin = spectrogram_smooth[40 + i];
    bin = bin * 0.5 + (bin * bin) * 0.5;
    brightness_high += bin;
  }

  brightness_low /= (SQ15x16)10.0;
  brightness_mid /= (SQ15x16)10.0;
  brightness_high /= (SQ15x16)10.0;

  if(brightness_low > 1.0){
    brightness_low = 1.0;
  }
  if(brightness_mid > 1.0){
    brightness_mid = 1.0;
  }
  if(brightness_high > 1.0){
    brightness_high = 1.0;
  }

  for(uint8_t i = 0; i < CONFIG.SQUARE_ITER; i++){
    brightness_low *= brightness_low;
    brightness_mid *= brightness_mid;
    brightness_high *= brightness_high;
  }

  CRGB16 col = { brightness_low, brightness_mid, brightness_high };
  leds_16[63] = col;
  */

  CRGB16 sum_color;
  SQ15x16 share = 1 / 6.0;
  for (uint8_t i = 0; i < 12; i++) {
    float prog = i / 12.0;
    SQ15x16 bin = chromagram_smooth[i];
    CRGB16 add_color = hsv(prog, CONFIG.SATURATION, bin*bin * share);

    sum_color.r += add_color.r;
    sum_color.g += add_color.g;
    sum_color.b += add_color.b;
  }

  if (sum_color.r > 1.0) { sum_color.r = 1.0; };
  if (sum_color.g > 1.0) { sum_color.g = 1.0; };
  if (sum_color.b > 1.0) { sum_color.b = 1.0; };

  for (uint8_t i = 0; i < CONFIG.SQUARE_ITER; i++) {
    sum_color.r *= sum_color.r;
    sum_color.g *= sum_color.g;
    sum_color.b *= sum_color.b;
  }

  CRGB temp_col = { uint8_t(sum_color.r * 255), uint8_t(sum_color.g * 255), uint8_t(sum_color.b * 255) };
  temp_col = force_saturation(temp_col, 255*CONFIG.SATURATION);

  if (chromatic_mode == false) {
    SQ15x16 led_hue = chroma_val + hue_position + (sqrt(float(1.0)) * SQ15x16(0.05));
    temp_col = force_hue(temp_col, 255*float(led_hue));
  }

  leds_16[63] = { temp_col.r / 255.0, temp_col.g / 255.0, temp_col.b / 255.0 };
  leds_16[64] = leds_16[63];

  //-------------------------------------------------------

  // Copy last frame to temp
  memcpy(leds_16_prev, leds_16, sizeof(CRGB16) * NATIVE_RESOLUTION);

  for(uint8_t i = 0; i < 32; i++){
    float prog = i / 31.0;
    leds_16[128-1-i].r *= (prog*prog);
    leds_16[128-1-i].g *= (prog*prog);
    leds_16[128-1-i].b *= (prog*prog);
  }

  for (uint8_t i = 0; i < 64; i++) {
    leds_16[i] = leds_16[128 - 1 - i];
  }
}