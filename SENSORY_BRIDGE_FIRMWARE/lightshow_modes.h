// Default mode!
void light_mode_gdft() {
  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {  // 64 freqs
    float bin = note_spectrogram_smooth[i];
    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      bin = bin * bin;
    }
    uint16_t led_brightness = 255 * bin;

    brightness_levels[i] = led_brightness;  // Can use this value later if needed

    float led_hue = 21.33333333 * i;  // Makes hue completely cycle once per octave

    CRGB col = CRGB(0, 0, 0);
    hsv2rgb_spectrum(  // Spectrum has better low-light color resolution than default "rainbow" HSV behavior
      CHSV(led_hue, 255, brightness_levels[i]),
      col);

    leds[i * 2 + 0] = col;  // Two LEDs at a time
    leds[i * 2 + 1] = col;
  }
}

void light_mode_gdft_chromagram() {
  for (uint16_t i = 0; i < 128; i++) {
    float prog = i / 128.0;
    float level = interpolate(prog, note_chromagram, 12);

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER; s++) {
      level = level * level;
    }

    leds[i] = CHSV(255 * prog, 255, 255 * level);
  }
}

void light_mode_bloom(bool fast_scroll) {
  static uint32_t iter = 0;
  const float led_share = 255 / float(12);

  iter++;

  CRGB sum_color = CRGB(0, 0, 0);
  for (uint8_t i = 0; i < 12; i++) {
    float prog = i / float(12);
    float bin = note_chromagram[i];

    //bin *= 2.0;

    if (bin > 1.0) {
      bin = 1.0;
    }

    for (uint8_t s = 0; s < CONFIG.SQUARE_ITER+2; s++) {
      bin = bin * bin;
    }

    CRGB out_col;
    hsv2rgb_spectrum(
      CHSV(255 * prog, 255, led_share * bin),
      out_col);
    sum_color += out_col;
  }

  if (iter % 2 == 1 || fast_scroll == true) { // Render every other frame in slow mode, or all in fast mode
    for (uint8_t i = 0; i < 127; i++) {
      leds_temp[127 - i] = leds_last[127 - i - 1];
    }
  }
  else{ // Skip frame in slow mode
    memcpy(leds_temp, leds_last, sizeof(CRGB)*128);
  }
  leds_temp[0] = sum_color; // New information goes here

  load_leds_from_temp();
  save_leds_to_last();
}


// ----------------------------------------------------------------------------
//
// (Work in progress code below, may delete later)
//
// ----------------------------------------------------------------------------

/*

void light_mode_gdft_retro(uint32_t t_now_us) {
  // SPECTROGRAM
  for (uint8_t i = 0; i < NUM_FREQS; i += FREQUENCY_HOP) {
    retro_heat_targets[i] = note_spectrogram_smooth[i];
  }

  update_retro_heat(t_now_us);
  render_retro_heat();

  for (uint8_t i = 0; i < NUM_FREQS; i += 1) {
    float hue = (21.33333333) * i;
    CRGB filter = CHSV(hue, 215, 255);

    filter.r *=  0.5;

    float b_val = leds[i * 2 + 0].b / 255.0;
    leds[i * 2 + 0].b = sqrtf(b_val) * 255;

    leds[i * 2 + 0].r = (leds[i * 2 + 0].r * 0.05) + scale8(leds[i * 2 + 0].r, filter.r) * 0.95;
    leds[i * 2 + 0].g = (leds[i * 2 + 0].g * 0.05) + scale8(leds[i * 2 + 0].g, filter.g) * 0.95;
    leds[i * 2 + 0].b = (leds[i * 2 + 0].b * 0.05) + scale8(leds[i * 2 + 0].b, filter.b) * 0.95;

    leds[i * 2 + 1] = leds[i * 2 + 0];
  }
}

void light_mode_gdft_chromagram() {
  // CHROMAGRAM
  for (uint8_t i = 0; i < 128; i += 1) {
    float chroma_level = interpolate(i / 128.0, note_chromagram, 12);
    hsv2rgb_spectrum(
      CHSV(21.33333333333333 * (i / 10.0), 255, 255 * chroma_level),
      leds[i]
    );
  }
}

void light_mode_gdft_with_peak_color_spreading() {
  for (uint8_t i = 0; i < NUM_FREQS; i += FREQUENCY_HOP) {
    float bin = note_spectrogram_smooth[i];
    if (bin > 1.0) {
      bin = 1.0;
    }
    float bright = 255 * (bin * bin);
    brightness_levels[i] = bright;
  }

  enum peak_states {
    UNSOLVED,
    UNEXPANDED,
    EXPANDED
  };

  static float hue_positions_follower[NUM_FREQS] = { 0 };

  //Serial.println("IDENTIFY PEAKS -----------------------");

  uint8_t peak_positions   [NUM_FREQS] = { UNSOLVED };
  float   hue_positions    [NUM_FREQS] = { -1       };
  bool    solved_map       [NUM_FREQS] = { false    };

  bool debug_map[NUM_FREQS] = { false };

  uint8_t peak_index = 0;

  fadeToBlackBy( leds, 128, 255 );

  bool in_peak = false;
  uint8_t start_pos = 0;
  uint8_t end_pos = 0;
  bool    rising  = false;
  bool    falling = false;

  uint8_t last_val = 0;
  uint8_t peaks_found = 0;
  uint8_t peak_start = 0;
  uint8_t peak_end   = 0;
  bool peak_complete = false;

  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    if (brightness_levels[i] == 0) { // if blank, consider solved
      if (falling == true) {
        falling = false;
        peak_end = i;
        peak_complete = true;
      }
      else {
        peak_start = i;
      }
    }
    else { // if not blank (peak area)
      if (brightness_levels[i] > last_val) { // brightness rising
        if (rising == false) {
          if (falling == true) { // didn't fall to full zero, but this is the end of a peak
            falling = false;
            peak_end = i;
            peak_complete = true;
          }
          else {
            rising = true;
            peak_start = i;
          }
        }
      }
      else if (brightness_levels[i] < last_val) { // brightness falling
        if (rising == true) {
          rising = false;
          falling = true;
        }
      }
      else {
        // stayed the same since last pixel, above zero
      }
    }

    if (peak_complete == true) {
      peak_complete = false;
      uint8_t peak_center = (peak_start + peak_end) / 2.0;

      hue_positions[peak_center] = 21.33333333 * peak_center; // hit peak here, set color
      while (hue_positions[peak_center] >= 256.0) {
        hue_positions[peak_center] -= 256.0;
      }
      if (hue_positions[peak_center] > 255.0) {
        hue_positions[peak_center] = 255.0;
      }
      peak_positions[peak_center] = UNEXPANDED;
      debug_map[peak_center]  = true;
      peaks_found++;
      //Serial.print("PEAK FOUND AT ");
      //Serial.println(peak_center);
    }

    last_val = brightness_levels[i];
  }

  if (peaks_found > 0) {
    //Serial.println("SOLVE EXPANSION -----------------------");
    bool solved = false;
    uint8_t pointer_index = 0;
    while (solved == false) {
      int16_t this_index  = pointer_index;
      if (peak_positions[this_index] == UNEXPANDED) { // if this pixel is an unexpanded peak
        float expand_hue = hue_positions[this_index];

        uint16_t expansion_size = 1;
        bool peak_solved = false;
        while (peak_solved == false) {
          int16_t left_index  = pointer_index - expansion_size;
          int16_t right_index = pointer_index + expansion_size;

          if (left_index >= 0) {
            if (peak_positions[left_index ] != UNSOLVED) {
              peak_solved = true;
            }
            if (peak_positions[right_index] != UNSOLVED) {
              peak_solved = true;
            }
          }

          if (peak_solved == false) {
            expansion_size += 1;
          }
        }

        uint16_t half_expansion = expansion_size / 2;

        int16_t left_index  = pointer_index - half_expansion;
        int16_t right_index = pointer_index + half_expansion;

        if (left_index < 0) {
          left_index = 0;
        }
        if (right_index >= NUM_FREQS) {
          right_index = NUM_FREQS - 1;
        }

        for (uint8_t e = left_index; e <= right_index; e++) {
          hue_positions[e] = expand_hue;
        }

        peak_positions[this_index] = EXPANDED; // peak solved and expanded
      }

      pointer_index++;
      if (pointer_index >= NUM_FREQS) {
        pointer_index = 0;
      }

      solved = true; // temporary
      for (uint8_t i = 0; i < NUM_FREQS; i++) { // check for holes in expansion
        if (peak_positions[i] == UNEXPANDED) { // if hole, not solved yet
          solved = false;
        }
      }
    }
  }

  for (uint8_t i = 0; i < NUM_FREQS; i += FREQUENCY_HOP) {
    if (hue_positions[i] > hue_positions_follower[i]) {
      float delta = hue_positions[i] - hue_positions_follower[i];
      hue_positions_follower[i] += delta * 0.1;
    }
    if (hue_positions[i] < hue_positions_follower[i]) {
      float delta = hue_positions_follower[i] - hue_positions[i];
      hue_positions_follower[i] -= delta * 0.1;
    }
  }

  for (uint8_t i = 0; i < NUM_FREQS; i += FREQUENCY_HOP) {
    float   led_hue = 21.33333333 * i; //hue_positions_follower[i];
    uint8_t led_brightness = brightness_levels[i];

    CRGB col = CRGB(0, 0, 0);
    hsv2rgb_spectrum(
      CHSV(led_hue, 255, led_brightness),
      col
    );

    leds[i * 2 + 0] = col;
    leds[i * 2 + 1] = col;
  }
}

*/
