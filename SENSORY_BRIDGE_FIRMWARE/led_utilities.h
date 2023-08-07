extern void propagate_noise_cal();
extern void start_noise_cal();

enum blending_modes {
  BLEND_MIX,
  BLEND_ADD,
  BLEND_MULTIPLY,

  NUM_BLENDING_MODES  // used to know the length of this list if it changes in the future
};


CRGB16 interpolate_hue(SQ15x16 hue) {
  // Scale hue to [0, 63]
  SQ15x16 hue_scaled = hue * 63.0;

  // Calculate index1, avoiding expensive floor() call by using typecast to int
  int index1 = hue_scaled.getInteger();

  // If index1 < 0, make it 0. If index1 > 63, make it 63
  index1 = (index1 < 0 ? 0 : (index1 > 63 ? 63 : index1));

  // Calculate index2, minimizing the range to [index1, index1+1]
  int index2 = index1 + 1;

  // If index2 > 63, make it 63
  index2 = (index2 > 63 ? 63 : index2);

  // Compute interpolation factor
  SQ15x16 t = hue_scaled - SQ15x16(index1);
  SQ15x16 t_inv = SQ15x16(1.0) - t;

  CRGB16 output;
  output.r = t_inv * hue_lookup[index1][0] + t * hue_lookup[index2][0];
  output.g = t_inv * hue_lookup[index1][1] + t * hue_lookup[index2][1];
  output.b = t_inv * hue_lookup[index1][2] + t * hue_lookup[index2][2];

  return output;
}


CRGB16 desaturate(CRGB16 input_color, SQ15x16 amount) {
  SQ15x16 luminance = SQ15x16(0.2126) * input_color.r + SQ15x16(0.7152) * input_color.g + SQ15x16(0.0722) * input_color.b;
  SQ15x16 amount_inv = SQ15x16(1.0) - amount;

  CRGB16 output;
  output.r = input_color.r * amount_inv + luminance * amount;
  output.g = input_color.g * amount_inv + luminance * amount;
  output.b = input_color.b * amount_inv + luminance * amount;

  return output;
}

CRGB16 hsv(SQ15x16 h, SQ15x16 s, SQ15x16 v) {
  while (h > 1.0) { h -= 1.0; }
  while (h < 0.0) { h += 1.0; }

  CRGB base_color = CHSV(uint8_t(h * 255.0), uint8_t(s * 255.0), 255);

  CRGB16 col = { base_color.r / 255.0, base_color.g / 255.0, base_color.b / 255.0 };
  //col = desaturate(col, SQ15x16(1.0) - s);

  col.r *= v;
  col.g *= v;
  col.b *= v;

  return col;
}

void clip_led_values() {
  for (uint16_t i = 0; i < 128; i++) {
    if (leds_16[i].r > (SQ15x16)1.0) {
      leds_16[i].r = (SQ15x16)1.0;
    } else if (leds_16[i].r < (SQ15x16)0.0) {
      leds_16[i].r = (SQ15x16)0.0;
    }

    if (leds_16[i].g > (SQ15x16)1.0) {
      leds_16[i].g = (SQ15x16)1.0;
    } else if (leds_16[i].g < (SQ15x16)0.0) {
      leds_16[i].g = (SQ15x16)0.0;
    }

    if (leds_16[i].b > (SQ15x16)1.0) {
      leds_16[i].b = (SQ15x16)1.0;
    } else if (leds_16[i].b < (SQ15x16)0.0) {
      leds_16[i].b = (SQ15x16)0.0;
    }
  }
}

void reverse_leds(CRGB arr[], uint16_t size) {
  uint16_t start = 0;
  uint16_t end = size - 1;
  while (start < end) {
    CRGB temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
  }
}

void run_sweet_spot() {
  static float sweet_spot_brightness = 0.0;  // init to zero for first fade in

  if (sweet_spot_brightness < 1.0) {
    sweet_spot_brightness += 0.05;
  }

  if (sweet_spot_state > sweet_spot_state_follower) {
    float delta = sweet_spot_state - sweet_spot_state_follower;
    sweet_spot_state_follower += delta * 0.05;
  } else if (sweet_spot_state < sweet_spot_state_follower) {
    float delta = sweet_spot_state_follower - sweet_spot_state;
    sweet_spot_state_follower -= delta * 0.05;
  }

  uint16_t led_power[3] = { 0, 0, 0 };
  for (float i = -1; i <= 1; i++) {
    float position_delta = fabs(i - sweet_spot_state_follower);
    if (position_delta > 1.0) {
      position_delta = 1.0;
    }

    float led_level = 1.0 - position_delta;
    led_level *= led_level;
    //                                                Never fully dim
    led_power[uint8_t(i + 1)] = 256 * led_level * (0.1 + silent_scale * 0.9) * sweet_spot_brightness * (CONFIG.PHOTONS * CONFIG.PHOTONS);
  }

  ledcWrite(SWEET_SPOT_LEFT_CHANNEL, led_power[0]);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, led_power[1]);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, led_power[2]);
}

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0-1.0
CRGB lerp_led_OLD(float index, CRGB* led_array) {
  float NUM_LEDS_NATIVE = NATIVE_RESOLUTION - 1;  // count from zero
  float index_f = index * NUM_LEDS_NATIVE;
  if (index_f > NUM_LEDS_NATIVE) {
    return CRGB(0, 0, 0);
  }
  int index_i = (int)index_f;
  float index_f_frac = index_f - index_i;
  CRGB out_col = CRGB(0, 0, 0);
  out_col.r += (1 - index_f_frac) * led_array[index_i].r;
  out_col.g += (1 - index_f_frac) * led_array[index_i].g;
  out_col.b += (1 - index_f_frac) * led_array[index_i].b;

  out_col.r += index_f_frac * led_array[index_i + 1].r;
  out_col.g += index_f_frac * led_array[index_i + 1].g;
  out_col.b += index_f_frac * led_array[index_i + 1].b;
  return out_col;
}

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0-1.0
CRGB lerp_led_NEW(float index, CRGB* led_array) {
  const uint16_t NUM_LEDS_NATIVE = NATIVE_RESOLUTION - 1;  // count from zero
  uint32_t index_fp = (uint32_t)(index * (float)NUM_LEDS_NATIVE * 256.0f);

  if (index_fp > (NUM_LEDS_NATIVE << 8)) {
    return CRGB::Black;
  }

  uint16_t index_i = index_fp >> 8;
  uint8_t index_f_frac = index_fp & 0xFF;

  // Use FastLED built-in lerp8by8 function for interpolation
  CRGB out_col;
  out_col.r = lerp8by8(led_array[index_i].r, led_array[index_i + 1].r, index_f_frac);
  out_col.g = lerp8by8(led_array[index_i].g, led_array[index_i + 1].g, index_f_frac);
  out_col.b = lerp8by8(led_array[index_i].b, led_array[index_i + 1].b, index_f_frac);

  return out_col;
}

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0 - 128.0
CRGB16 lerp_led_16(SQ15x16 index, CRGB16* led_array) {
  int32_t index_whole = index.getInteger();
  SQ15x16 index_fract = index - (SQ15x16)index_whole;

  int32_t index_left = index_whole + 0;
  int32_t index_right = index_whole + 1;

  SQ15x16 mix_left = SQ15x16(1.0) - index_fract;
  SQ15x16 mix_right = SQ15x16(1.0) - mix_left;

  CRGB16 out_col;
  out_col.r = led_array[index_left].r * mix_left + led_array[index_right].r * mix_right;
  out_col.g = led_array[index_left].g * mix_left + led_array[index_right].g * mix_right;
  out_col.b = led_array[index_left].b * mix_left + led_array[index_right].b * mix_right;

  return out_col;
}

void apply_brightness() {
  // This is only used to fade in when booting!
  if (millis() >= 1000 && noise_transition_queued == false && mode_transition_queued == false) {
    if (MASTER_BRIGHTNESS < 1.0) {
      MASTER_BRIGHTNESS += 0.005;
    }
    if (MASTER_BRIGHTNESS > 1.0) {
      MASTER_BRIGHTNESS = 1.00;
    }
  }

  SQ15x16 brightness = MASTER_BRIGHTNESS * (CONFIG.PHOTONS * CONFIG.PHOTONS) * silent_scale;

  for (uint16_t i = 0; i < 128; i++) {
    leds_16[i].r *= brightness;
    leds_16[i].g *= brightness;
    leds_16[i].b *= brightness;
  }

  clip_led_values();
}

void quantize_color(bool temporal_dithering) {
  if (temporal_dithering) {
    dither_step++;
    if (dither_step >= 4) {
      dither_step = 0;
    }

    static uint8_t noise_origin_r = 0;  // 0
    static uint8_t noise_origin_g = 0;  // 2
    static uint8_t noise_origin_b = 0;  // 4

    noise_origin_r += 1;
    noise_origin_g += 1;
    noise_origin_b += 1;

    for (uint16_t i = 0; i < CONFIG.LED_COUNT; i += 1) {
  	  uint16_t out_i = i;
#if defined(LUMOSSTICK)
      if (CONFIG.LUMOS_ORDER) {
    	  // we map the rows of LumosStick LEDs
        out_i = ((i % LUMOS_SEGMENTS) * LUMOS_LEN) + (i / LUMOS_SEGMENTS);
      }
#endif

      // RED #####################################################
      SQ15x16 decimal_r = leds_scaled[i].r * SQ15x16(254);
      SQ15x16 whole_r = decimal_r.getInteger();
      SQ15x16 fract_r = decimal_r - whole_r;

      if (fract_r >= dither_table[(noise_origin_r + i) % 4]) {
        whole_r += SQ15x16(1);
      }

      leds_out[out_i].r = whole_r.getInteger();

      // GREEN ###################################################
      SQ15x16 decimal_g = leds_scaled[i].g * SQ15x16(254);
      SQ15x16 whole_g = decimal_g.getInteger();
      SQ15x16 fract_g = decimal_g - whole_g;

      if (fract_g >= dither_table[(noise_origin_g + i) % 4]) {
        whole_g += SQ15x16(1);
      }

      leds_out[out_i].g = whole_g.getInteger();

      // BLUE ####################################################
      SQ15x16 decimal_b = leds_scaled[i].b * SQ15x16(254);
      SQ15x16 whole_b = decimal_b.getInteger();
      SQ15x16 fract_b = decimal_b - whole_b;

      if (fract_b >= dither_table[(noise_origin_b + i) % 4]) {
        whole_b += SQ15x16(1);
      }

      leds_out[out_i].b = whole_b.getInteger();
    }
  } else {
    for (uint16_t i = 0; i < CONFIG.LED_COUNT; i += 1) {
  	  uint16_t out_i = i;
#if defined(LUMOSSTICK)
      if (CONFIG.LUMOS_ORDER) {
    	  // we map the rows of LumosStick LEDs
        out_i = ((i % LUMOS_SEGMENTS) * LUMOS_LEN) + (i / LUMOS_SEGMENTS);
      }
#endif
      leds_out[out_i].r = uint8_t(leds_scaled[i].r * 255);
      leds_out[out_i].g = uint8_t(leds_scaled[i].g * 255);
      leds_out[out_i].b = uint8_t(leds_scaled[i].b * 255);
    }
  }

#if 0
  if (CONFIG.LED_COUNT >= 240) {
    // TEMPORARY HACK: the last two LEDs are calculated incorrectly
    // it is likely some artifact of the 280 LEDs vs the original 128
    uint16_t src_i = (CONFIG.LED_COUNT - 3);
#if defined(LUMOSSTICK)
    if (CONFIG.LUMOS_ORDER) {
        // we map the rows of LumosStick LEDs
      src_i = ((src_i % LUMOS_SEGMENTS) * LUMOS_LEN) + (src_i / LUMOS_SEGMENTS);
    }
#endif
    for (uint16_t i = 0; i < 2; i++) {
      uint16_t tgt_i = (CONFIG.LED_COUNT - 1) - i;
#if defined(LUMOSSTICK)
      if (CONFIG.LUMOS_ORDER) {
        // we map the rows of LumosStick LEDs
        tgt_i = ((tgt_i % LUMOS_SEGMENTS) * LUMOS_LEN) + (tgt_i / LUMOS_SEGMENTS);
      }
#endif
      // copy the last good LED to the otehr two
      leds_out[tgt_i].r = leds_out[src_i].r;
      leds_out[tgt_i].g = leds_out[src_i].g;
      leds_out[tgt_i].b = leds_out[src_i].b;
    }
  }
#endif
}

void apply_incandescent_filter() {
  SQ15x16 mix = CONFIG.INCANDESCENT_FILTER;
  SQ15x16 inv_mix = 1.0 - mix;

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    SQ15x16 filtered_r = leds_16[i].r * incandescent_lookup.r;
    SQ15x16 filtered_g = leds_16[i].g * incandescent_lookup.g;
    SQ15x16 filtered_b = leds_16[i].b * incandescent_lookup.b;

    leds_16[i].r = (leds_16[i].r * inv_mix) + (filtered_r * mix);
    leds_16[i].g = (leds_16[i].g * inv_mix) + (filtered_g * mix);
    leds_16[i].b = (leds_16[i].b * inv_mix) + (filtered_b * mix);

    /*
    SQ15x16 max_val = 0;
    if (leds_16[i].r > max_val) { max_val = leds_16[i].r; }
    if (leds_16[i].g > max_val) { max_val = leds_16[i].g; }
    if (leds_16[i].b > max_val) { max_val = leds_16[i].b; }

    SQ15x16 leakage = (max_val >> 2) * mix;

    CRGB base_col = CRGB(
      (uint16_t(leds[i].r * (255 - leakage)) >> 8),
      (uint16_t(leds[i].g * (255 - leakage)) >> 8),
      (uint16_t(leds[i].b * (255 - leakage)) >> 8));

    CRGB leak_col = CRGB(
      uint16_t((uint16_t(incandescent_lookup.r * (leakage)) >> 8) * max_val) >> 8,
      uint16_t((uint16_t(incandescent_lookup.g * (leakage)) >> 8) * max_val) >> 8,
      uint16_t((uint16_t(incandescent_lookup.b * (leakage)) >> 8) * max_val) >> 8);

    leds[i].r = base_col.r + leak_col.r;
    leds[i].g = base_col.g + leak_col.g;
    leds[i].b = base_col.b + leak_col.b;
    */
  }
}

void set_dot_position(uint16_t dot_index, SQ15x16 new_pos) {
  dots[dot_index].last_position = dots[dot_index].position;
  dots[dot_index].position = new_pos;
}

void draw_line(CRGB16* layer, SQ15x16 x1, SQ15x16 x2, CRGB16 color, SQ15x16 alpha) {
  bool lighten = true;
  if (color.r == 0 && color.g == 0 && color.b == 0) {
    lighten = false;
  }

  x1 *= (SQ15x16)(128 - 1);
  x2 *= (SQ15x16)(128 - 1);

  if (x1 > x2) {  // Ensure x1 <= x2
    SQ15x16 temp = x1;
    x1 = x2;
    x2 = temp;
  }

  SQ15x16 ix1 = floorFixed(x1);
  SQ15x16 ix2 = ceilFixed(x2);

  // start pixel
  if (ix1 >= 0 && ix1 < 128) {
    SQ15x16 coverage = 1.0 - (x1 - ix1);
    SQ15x16 mix = alpha * coverage;

    if (lighten == true) {
      layer[ix1.getInteger()].r += color.r * mix;
      layer[ix1.getInteger()].g += color.g * mix;
      layer[ix1.getInteger()].b += color.b * mix;
    } else {
      layer[ix1.getInteger()].r = layer[ix1.getInteger()].r * (1.0 - mix) + color.r * mix;
      layer[ix1.getInteger()].g = layer[ix1.getInteger()].g * (1.0 - mix) + color.g * mix;
      layer[ix1.getInteger()].b = layer[ix1.getInteger()].b * (1.0 - mix) + color.b * mix;
    }
  }

  // end pixel
  if (ix2 >= 0 && ix2 < 128) {
    SQ15x16 coverage = x2 - floorFixed(x2);
    SQ15x16 mix = alpha * coverage;

    if (lighten == true) {
      layer[ix2.getInteger()].r += color.r * mix;
      layer[ix2.getInteger()].g += color.g * mix;
      layer[ix2.getInteger()].b += color.b * mix;
    } else {
      layer[ix2.getInteger()].r = layer[ix2.getInteger()].r * (1.0 - mix) + color.r * mix;
      layer[ix2.getInteger()].g = layer[ix2.getInteger()].g * (1.0 - mix) + color.g * mix;
      layer[ix2.getInteger()].b = layer[ix2.getInteger()].b * (1.0 - mix) + color.b * mix;
    }
  }

  // pixels in between
  for (SQ15x16 i = ix1 + 1; i < ix2; i++) {
    if (i >= 0 && i < 128) {
      layer[i.getInteger()].r += color.r * alpha;
      layer[i.getInteger()].g += color.g * alpha;
      layer[i.getInteger()].b += color.b * alpha;

      if (lighten == true) {
        layer[i.getInteger()].r += color.r * alpha;
        layer[i.getInteger()].g += color.g * alpha;
        layer[i.getInteger()].b += color.b * alpha;
      } else {
        layer[i.getInteger()].r = layer[i.getInteger()].r * (1.0 - alpha) + color.r * alpha;
        layer[i.getInteger()].g = layer[i.getInteger()].g * (1.0 - alpha) + color.g * alpha;
        layer[i.getInteger()].b = layer[i.getInteger()].b * (1.0 - alpha) + color.b * alpha;
      }
    }
  }
}

void draw_dot(CRGB16* layer, uint16_t dot_index, CRGB16 color) {
  SQ15x16 position = dots[dot_index].position;
  SQ15x16 last_position = dots[dot_index].last_position;

  SQ15x16 positional_distance = fabs_fixed(position - last_position);
  if (positional_distance < 1.0) {
    positional_distance = 1.0;
  }

  SQ15x16 net_brightness_per_pixel = 1.0 / positional_distance;
  if (net_brightness_per_pixel > 1.0) {
    net_brightness_per_pixel = 1.0;
  }

  draw_line(
    layer,
    position,
    last_position,
    color,
    net_brightness_per_pixel);
}

void render_photons_graph() {
  // Draw graph ticks
  uint8_t ticks = 5;
  SQ15x16 tick_distance = (0.425 / (ticks - 1));
  SQ15x16 tick_pos = 0.025;

  CRGB16 background = { 0.0, 0.0, 0.0 };
  CRGB16 needle_color = { incandescent_lookup.r * incandescent_lookup.r * 0.9, incandescent_lookup.g * incandescent_lookup.g * 0.9, incandescent_lookup.b * incandescent_lookup.b * 0.9 };

  //draw_line(leds_16_ui, 0.0, 0.5, background, 1.0);

  memset(leds_16_ui, 0, sizeof(CRGB16) * 128);

  for (uint8_t i = 0; i < ticks; i++) {
    SQ15x16 prog = i / float(ticks);
    SQ15x16 tick_brightness = 0.2 + 0.4 * prog;
    tick_brightness *= tick_brightness;
    tick_brightness *= tick_brightness;
    CRGB16 tick_color = { 1.0 * tick_brightness, 0, 0 };

    set_dot_position(GRAPH_DOT_1 + i, tick_pos);
    draw_dot(leds_16_ui, GRAPH_DOT_1 + i, tick_color);
    tick_pos += tick_distance;
  }

  SQ15x16 needle_pos = 0.025 + (0.425 * CONFIG.PHOTONS);

  // Draw needle
  set_dot_position(GRAPH_NEEDLE, needle_pos);
  draw_dot(leds_16_ui, GRAPH_NEEDLE, needle_color);
}

void render_chroma_graph() {
  memset(leds_16_ui, 0, sizeof(CRGB16) * 128);

  SQ15x16 half_height = 128 >> 1;
  SQ15x16 quarter_height = 128 >> 2;

  if (chromatic_mode == false) {
    for (SQ15x16 i = 5; i < half_height - 5; i++) {
      SQ15x16 prog = i / half_height;

      SQ15x16 distance_to_center = fabs_fixed(i - quarter_height);
      SQ15x16 brightness;  // = SQ15x16(1.0) - distance_to_center / quarter_height;

      if (distance_to_center < 3) {
        brightness = 1.0;
      } else if (distance_to_center < 5) {
        brightness = 0.0;
      } else {
        brightness = 0.20;
      }

      leds_16_ui[i.getInteger()] = hsv((SQ15x16(chroma_val + hue_position) - 0.48) + prog, CONFIG.SATURATION, brightness * brightness);
    }
  } else {
    SQ15x16 dot_pos = 0.025;
    SQ15x16 dot_distance = (0.425 / (12 - 1));

    static float radians = 0.0;
    radians -= 0.025;

    for (uint8_t i = 0; i < 12; i++) {
      SQ15x16 wave = sin(radians + (i * 0.5)) * 0.4 + 0.6;

      CRGB16 dot_color = hsv(SQ15x16(i / 12.0), CONFIG.SATURATION, wave * wave);
      set_dot_position(MAX_DOTS - 1 - i, dot_pos);
      draw_dot(leds_16_ui, MAX_DOTS - 1 - i, dot_color);

      dot_pos += dot_distance;
    }
  }
}

void render_mood_graph() {
  // Draw graph ticks
  uint8_t ticks = 5;
  SQ15x16 tick_distance = (0.425 / (ticks - 1));
  SQ15x16 tick_pos = 0.025;

  CRGB16 background = { 0.0, 0.0, 0.0 };
  CRGB16 needle_color = { incandescent_lookup.r * incandescent_lookup.r * 0.9, incandescent_lookup.g * incandescent_lookup.g * 0.9, incandescent_lookup.b * incandescent_lookup.b * 0.9 };

  //draw_line(leds_16_ui, 0.0, 0.5, background, 1.0);

  memset(leds_16_ui, 0, sizeof(CRGB16) * 128);

  static float radians = 0.0;
  radians -= 0.02;

  for (uint8_t i = 0; i < ticks; i++) {
    SQ15x16 tick_brightness = 0.1;  // + (0.025 * sin(radians * (1 << i)));  // + (0.04 * sin(radians * ((i<<1)+1)));
    SQ15x16 mix = i / float(ticks - 1);

    CRGB16 tick_color = { tick_brightness * mix, 0.05 * tick_brightness, tick_brightness * (1.0 - mix) };

    set_dot_position(GRAPH_DOT_1 + i, tick_pos + (0.008 * sin(radians * (1 << i))));
    draw_dot(leds_16_ui, GRAPH_DOT_1 + i, tick_color);
    tick_pos += tick_distance;
  }

  SQ15x16 needle_pos = 0.025 + (0.425 * CONFIG.MOOD);

  // Draw needle
  set_dot_position(GRAPH_NEEDLE, needle_pos);
  draw_dot(leds_16_ui, GRAPH_NEEDLE, needle_color);
}

void transition_ui_mask_to_height(SQ15x16 target_height) {
  SQ15x16 distance = fabs_fixed(ui_mask_height - target_height);
  if (ui_mask_height > target_height) {
    ui_mask_height -= distance * 0.05;
  } else if (ui_mask_height < target_height) {
    ui_mask_height += distance * 0.05;
  }

  if (ui_mask_height < 0.0) {
    ui_mask_height = 0.0;
  } else if (ui_mask_height > 1.0) {
    ui_mask_height = 1.0;
  }

  memset(ui_mask, 0, sizeof(SQ15x16) * 128);
  for (uint8_t i = 0; i < 128 * ui_mask_height; i++) {
    ui_mask[i] = SQ15x16(1.0);
  }
}

void render_noise_cal() {
  // Noise cal UI
  float noise_cal_progress = noise_iterations / 256.0;

  uint8_t prog_led_index = (NATIVE_RESOLUTION >> 1) * noise_cal_progress;
  float max_val = 0.0;
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    if (noise_samples[i] > max_val) {
      max_val = float(noise_samples[i]);
    }
  }
  for (uint16_t i = 0; i < NATIVE_RESOLUTION >> 1; i++) {
    if (i < prog_led_index) {
      float led_level = float(noise_samples[i]) / max_val;
      led_level = led_level * 0.9 + 0.1;
      leds_16_ui[64 + i] = hsv(0.859, CONFIG.SATURATION, led_level * led_level);
      leds_16_ui[63 - i] = leds_16_ui[64 + i];
    } else if (i == prog_led_index) {
      leds_16_ui[64 + i] = hsv(0.875, 1.0, 1.0);
      leds_16_ui[63 - i] = leds_16_ui[64 + i];

      ui_mask[64 + i] = 1.0;
      ui_mask[63 - i] = ui_mask[64 + i];

    } else {
      leds_16_ui[64 + i] = hsv(0, 0, 0);
      leds_16_ui[63 - i] = leds_16_ui[64 + i];
    }
  }

  if (noise_iterations > 192) {  // fade out towards end of calibration
    uint16_t iters_left = 64 - (noise_iterations - 192);
    float brightness_level = iters_left / 64.0;
    brightness_level *= brightness_level;
  }
}

void render_ui() {
  if (noise_complete == true) {
    if (current_knob == K_NONE) {
      // Close UI if open
      if (ui_mask_height > 0.005) {
        transition_ui_mask_to_height(0.0);
      }
    } else {
      if (current_knob == K_PHOTONS) {
        render_photons_graph();
      } else if (current_knob == K_CHROMA) {
        render_chroma_graph();
      } else if (current_knob == K_MOOD) {
        render_mood_graph();
      }

      // Open UI if closed
      transition_ui_mask_to_height(0.5);
    }
  } else {
    render_noise_cal();
  }

  if (ui_mask_height > 0.005 || noise_complete == false) {
    for (uint8_t i = 0; i < 128; i++) {
      SQ15x16 mix = ui_mask[i];
      SQ15x16 mix_inv = SQ15x16(1.0) - mix;

      if (mix > 0.0) {
        leds_16[i].r = leds_16[i].r * mix_inv + leds_16_ui[i].r * mix;
        leds_16[i].g = leds_16[i].g * mix_inv + leds_16_ui[i].g * mix;
        leds_16[i].b = leds_16[i].b * mix_inv + leds_16_ui[i].b * mix;
      }
    }
  }
}

void scale_to_strip() {
  if (CONFIG.LED_COUNT == NATIVE_RESOLUTION) {
    for (SQ15x16 i = 0; i < CONFIG.LED_COUNT; i++) {
      //memcpy(leds_scaled, leds_16, sizeof(CRGB16)*NATIVE_RESOLUTION);
      leds_scaled[i.getInteger()] = leds_16[i.getInteger()];
    }
  } else {
    for (SQ15x16 i = 0; i < CONFIG.LED_COUNT; i++) {
      SQ15x16 prog = i / SQ15x16(CONFIG.LED_COUNT);

      // index is in the range of 0.0 - 128.0
      leds_scaled[i.getInteger()] = lerp_led_16(prog * SQ15x16(128.0), leds_16);
    }
  }
}

void show_leds() {
  apply_brightness();

  // Tint the color image with an incandescent LUT to reduce harsh blues
  if (CONFIG.INCANDESCENT_FILTER > 0.0) {
    apply_incandescent_filter();
  }

  if (CONFIG.BASE_COAT == true) {
    if (CONFIG.PHOTONS <= 0.05) {
      base_coat_width_target = 0.0;
    } else {
      base_coat_width_target = 1.0;
    }

    SQ15x16 transition_speed = 0.05;
    if (base_coat_width < base_coat_width_target) {
      base_coat_width += (base_coat_width_target - base_coat_width) * transition_speed;
    } else if (base_coat_width > base_coat_width_target) {
      base_coat_width -= (base_coat_width - base_coat_width_target) * transition_speed;
    }

    SQ15x16 backdrop_divisor = 256.0;

    SQ15x16 bottom_value_r = 1 / backdrop_divisor;
    SQ15x16 bottom_value_g = 1 / backdrop_divisor;
    SQ15x16 bottom_value_b = 1 / backdrop_divisor;

    CRGB16 backdrop_color = { bottom_value_r, bottom_value_g, bottom_value_b };

    SQ15x16 base_coat_width_scaled = base_coat_width * silent_scale;

    if (base_coat_width_scaled > 0.01) {
      draw_line(leds_16, 0.5 - (base_coat_width_scaled * 0.5), 0.5 + (base_coat_width_scaled * 0.5), backdrop_color, 1.0);
    }

    /*
    for (uint8_t i = 0; i < 128; i++) {
      if (leds_16[i].r < bottom_value_r) { leds_16[i].r = bottom_value_r; }
      if (leds_16[i].g < bottom_value_g) { leds_16[i].g = bottom_value_g; }
      if (leds_16[i].b < bottom_value_b) { leds_16[i].b = bottom_value_b; }
    }
    */
  }

  render_ui();
  clip_led_values();

  scale_to_strip();

  quantize_color(CONFIG.TEMPORAL_DITHERING);

  if (CONFIG.REVERSE_ORDER == true) {
    reverse_leds(leds_out, CONFIG.LED_COUNT);
  }

  FastLED.setDither(false);
  FastLED.show();
}

void init_leds() {
  bool leds_started = false;

  USBSerial.print("INIT_LEDS: ");

  leds_scaled = new CRGB16[CONFIG.LED_COUNT];
  leds_out = new CRGB[CONFIG.LED_COUNT];

#ifdef LUMOSSTICK
  USBSerial.print(" LUMOSSTICK ");
  if (CONFIG.LED_COUNT != (LUMOS_LEN * LUMOS_SEGMENTS)) {
      USBSerial.println(" LED COUNT MISMATCH ");
      delay(100); // allow time for serial messages to be transmitted      
  }

	// can not do this in a loop since the FastLED declaration is performed at compile time
	FastLED.addLeds<WS2812B, LED_DATA_PIN+0, GRB>(leds_out, 0*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+1, GRB>(leds_out, 1*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+2, GRB>(leds_out, 2*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+3, GRB>(leds_out, 3*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+4, GRB>(leds_out, 4*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+5, GRB>(leds_out, 5*LUMOS_LEN, LUMOS_LEN);
	FastLED.addLeds<WS2812B, LED_DATA_PIN+6, GRB>(leds_out, 6*LUMOS_LEN, LUMOS_LEN);

#else

  if (CONFIG.LED_TYPE == LED_NEOPIXEL) {
    if (CONFIG.LED_COLOR_ORDER == RGB) {
      FastLED.addLeds<WS2812B, LED_DATA_PIN, RGB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == GRB) {
      FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == BGR) {
      FastLED.addLeds<WS2812B, LED_DATA_PIN, BGR>(leds_out, CONFIG.LED_COUNT);
    }
  }

  else if (CONFIG.LED_TYPE == LED_DOTSTAR) {
#ifdef  LED_CLOCK_PIN
    if (CONFIG.LED_COLOR_ORDER == RGB) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == GRB) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, GRB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == BGR) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, BGR>(leds_out, CONFIG.LED_COUNT);
    }
#else
    USBSerial.println("ERROR: No LED_CLOCK_PIN defined");
#endif
  }

#endif

  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, CONFIG.MAX_CURRENT_MA); // we do our own dithering so this does not have any effect

  for (uint16_t x = 0; x < CONFIG.LED_COUNT; x++) {
    leds_out[x] = CRGB(0, 0, 0);
  }
  show_leds();

  leds_started = true;

  USBSerial.println(leds_started == true ? PASS : FAIL);
}

void blocking_flash(CRGB16 col) {
  led_thread_halt = true;
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds_16[i] = { 0, 0, 0 };
  }

  const uint8_t flash_times = 2;
  for (uint8_t f = 0; f < flash_times; f++) {
    for (uint8_t i = 0 + 48; i < NATIVE_RESOLUTION - 48; i++) {
      leds_16[i] = col;
    }
    show_leds();
    FastLED.delay(150);

    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      leds_16[i] = { 0, 0, 0 };
    }
    show_leds();
    FastLED.delay(150);
  }
  led_thread_halt = false;
}

void clear_all_led_buffers() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds_16[i] = { 0, 0, 0 };
    leds_16_temp[i] = { 0, 0, 0 };
    leds_16_fx[i] = { 0, 0, 0 };
  }

  for (uint16_t i = 0; i < CONFIG.LED_COUNT; i++) {
    leds_scaled[i] = { 0, 0, 0 };
    leds_out[i] = CRGB(0, 0, 0);
  }
}

void scale_image_to_half(CRGB16* led_array) {
  for (uint16_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    leds_16_temp[i].r = led_array[i << 1].r * SQ15x16(0.5) + led_array[(i << 1) + 1].r * SQ15x16(0.5);
    leds_16_temp[i].g = led_array[i << 1].g * SQ15x16(0.5) + led_array[(i << 1) + 1].g * SQ15x16(0.5);
    leds_16_temp[i].b = led_array[i << 1].b * SQ15x16(0.5) + led_array[(i << 1) + 1].b * SQ15x16(0.5);
    leds_16_temp[64 + i] = { 0, 0, 0 };
  }

  memcpy(led_array, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}

void unmirror() {
  for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {  // Interpolation
    SQ15x16 index = 64 + (i / 2.0);

    int32_t index_whole = index.getInteger();
    SQ15x16 index_fract = index - (SQ15x16)index_whole;

    int32_t index_left = index_whole + 0;
    int32_t index_right = index_whole + 1;

    SQ15x16 mix_left = SQ15x16(1.0) - index_fract;
    SQ15x16 mix_right = SQ15x16(1.0) - mix_left;

    CRGB16 out_col;
    out_col.r = leds_16[index_left].r * mix_left + leds_16[index_right].r * mix_right;
    out_col.g = leds_16[index_left].g * mix_left + leds_16[index_right].g * mix_right;
    out_col.b = leds_16[index_left].b * mix_left + leds_16[index_right].b * mix_right;

    leds_16_temp[i] = out_col;
  }

  memcpy(leds_16, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}

void shift_leds_up(CRGB16* led_array, uint16_t offset) {
  memcpy(leds_16_temp, led_array, sizeof(CRGB16) * NATIVE_RESOLUTION);
  memcpy(led_array + offset, leds_16_temp, (NATIVE_RESOLUTION - offset) * sizeof(CRGB16));
  memset(led_array, 0, offset * sizeof(CRGB16));
}

void shift_leds_down(CRGB* led_array, uint16_t offset) {
  memcpy(led_array, led_array + offset, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
  memset(led_array + (NATIVE_RESOLUTION - offset), 0, offset * sizeof(CRGB));
}

void mirror_image_downwards(CRGB16* led_array) {
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    leds_16_temp[64 + i] = led_array[64 + i];
    leds_16_temp[63 - i] = led_array[64 + i];
  }

  memcpy(led_array, leds_16_temp, sizeof(CRGB16) * NATIVE_RESOLUTION);
}

void intro_animation() {
  MASTER_BRIGHTNESS = 1.0;
  ledcWrite(SWEET_SPOT_LEFT_CHANNEL, 0.0 * 4096);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, 0.0 * 4096);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, 0.0 * 4096);

  for (float progress = 0.3; progress <= 0.925; progress += 0.01) {
    float total_vals = 0.925 - 0.3;
    float brightness = (progress - 0.3) / total_vals;

    brightness *= brightness;

    MASTER_BRIGHTNESS = brightness;
    ledcWrite(SWEET_SPOT_LEFT_CHANNEL, brightness * 4096);
    ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, brightness * 4096);

    float pos = (cos(progress * 5) + 1) / 2.0;
    float pos_whole = pos * NATIVE_RESOLUTION;
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      float delta = fabs(pos_whole - i);
      if (delta > 5.0) {
        delta = 5.0;
      }
      float led_level = 1.0 - (delta / 5.0);
      CRGB16 out_col = hsv(progress, 0, led_level);
      leds_16[i] = out_col;
    }
    show_leds();
  }

  clear_all_led_buffers();

  const uint8_t particle_count = 16;
  struct particle {
    float phase;
    float speed;
    CRGB16 col;
  };
  particle particles[particle_count*2];

  for (uint8_t p = 0; p < particle_count; p++) {
    float prog = p / float(particle_count);
    particles[p].phase = 0.0;
    particles[p].speed = 0.002 * (p + 1);
    particles[p].col = hsv(prog, CONFIG.SATURATION, 1.0);
  }
  MASTER_BRIGHTNESS = 1.0;
  float center_brightness = 0.0;

  for (uint16_t j = 0; j < 50; j++) {
    if (center_brightness < 1.0) {
      center_brightness += 0.2;
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, (center_brightness * center_brightness) * 4096);
    }

    float dimming = 1.0;
    float anim_prog = j / 50.0;
    if (anim_prog >= 0.5) {
      anim_prog = (anim_prog - 0.5) * 2.0;
      dimming = 1.0 - anim_prog;
      dimming *= dimming;
      MASTER_BRIGHTNESS = dimming;
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL, 0);
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, dimming * 4096);
      ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, 0);
    } else {
      anim_prog *= 2.0;
      dimming = 1.0 - anim_prog;
      dimming *= dimming;
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL, dimming * 4096);
      ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, dimming * 4096);
    }

    clear_all_led_buffers();

    for (uint8_t p = 0; p < particle_count; p++) {
      particles[p].phase += particles[p].speed;

      float pos = (sin(particles[p].phase * 5) + 1) / 2.0;
      float pos_whole = pos * NATIVE_RESOLUTION;
      for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
        float delta = fabs(pos_whole - i);
        if (delta > 10.0) {
          delta = 10.0;
        }
        float led_level = 1.0 - (delta / 10.0);
        led_level *= led_level;
        CRGB16 out_col = particles[p].col;
        out_col.r *= led_level;
        out_col.g *= led_level;
        out_col.b *= led_level;
        leds_16[i].r += out_col.r;
        leds_16[i].g += out_col.g;
        leds_16[i].b += out_col.b;
      }
    }
    show_leds();
    FastLED.delay(1);
  }

  MASTER_BRIGHTNESS = 0.0;
  ledcWrite(SWEET_SPOT_LEFT_CHANNEL, 0);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, 0);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL, 0);
}

void run_transition_fade() {
  if (MASTER_BRIGHTNESS > 0.0) {
    MASTER_BRIGHTNESS -= 0.02;

    if (MASTER_BRIGHTNESS < 0.0) {
      MASTER_BRIGHTNESS = 0.0;
    }
  } else {
    if (mode_transition_queued == true) {  // If transition for MODE button press
      mode_transition_queued = false;
      if (mode_destination == -1) {  // Triggered via button
        CONFIG.LIGHTSHOW_MODE++;
        if (CONFIG.LIGHTSHOW_MODE >= NUM_MODES) {
          CONFIG.LIGHTSHOW_MODE = 0;
        }
      } else {  // Triggered via Serial
        CONFIG.LIGHTSHOW_MODE = mode_destination;
        mode_destination = -1;
      }
    }

    if (noise_transition_queued == true) {  // If transition for NOISE button press
      noise_transition_queued = false;
      // start noise cal
      if (debug_mode) {
        USBSerial.println("COLLECTING AMBIENT NOISE SAMPLES...");
      }
      propagate_noise_cal();
      start_noise_cal();
    }
  }
}

/*
void distort_exponential() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION - 1);
    float prog_distorted = prog * prog;
    leds_fx[i] = lerp_led_NEW(prog_distorted, leds);
  }
  load_leds_from_fx();
}

void distort_logarithmic() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION - 1);
    float prog_distorted = sqrt(prog);
    leds_fx[i] = lerp_led_NEW(prog_distorted, leds);
  }
  load_leds_from_fx();
}

void increase_saturation(uint8_t amount) {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    CHSV hsv = rgb2hsv_approximate(leds[i]);
    hsv.s = qadd8(hsv.s, amount);
    leds[i] = hsv;
  }
}

void fade_top_half(bool shifted = false) {
  int16_t shift = 0;
  if (shifted == true) {
    shift -= 64;
  }
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    float fade = i / float(NATIVE_RESOLUTION >> 1);

    leds[(NATIVE_RESOLUTION - 1 - i) + shift].r *= fade;
    leds[(NATIVE_RESOLUTION - 1 - i) + shift].g *= fade;
    leds[(NATIVE_RESOLUTION - 1 - i) + shift].b *= fade;
  }
}
*/

float apply_contrast_float(float value, float intensity) {
  float mid_point = 0.5;
  float factor = (intensity * 2.0) + 1.0;

  float contrasted_value = (value - mid_point) * factor + mid_point;
  contrasted_value = constrain(contrasted_value, 0.0, 1.0);

  return contrasted_value;
}

SQ15x16 apply_contrast_fixed(SQ15x16 value, SQ15x16 intensity) {
  SQ15x16 mid_point = 0.5;
  SQ15x16 factor = (intensity * 2.0) + 1.0;

  SQ15x16 contrasted_value = (value - mid_point) * factor + mid_point;
  if (contrasted_value > SQ15x16(1.0)) {
    contrasted_value = 1.0;
  } else if (contrasted_value < SQ15x16(0.0)) {
    contrasted_value = 0.0;
  }

  return contrasted_value;
}

#include <stdint.h>

uint8_t apply_contrast(uint8_t value, uint8_t intensity) {
  uint16_t mid_point = 128;
  uint16_t factor = (uint16_t)intensity + 1;

  int16_t contrasted_value = (int16_t)value - mid_point;
  contrasted_value = (contrasted_value * factor) + (mid_point << 8);
  contrasted_value >>= 8;

  if (contrasted_value < 0) {
    contrasted_value = 0;
  } else if (contrasted_value > 255) {
    contrasted_value = 255;
  }

  return (uint8_t)contrasted_value;
}

/*
void force_incandescent_output() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    uint8_t max_val = 0;
    if (leds[i].r > max_val) { max_val = leds[i].r; }
    if (leds[i].g > max_val) { max_val = leds[i].g; }
    if (leds[i].b > max_val) { max_val = leds[i].b; }

    leds[i] = CRGB(
      uint16_t(incandescent_lookup.r * max_val) >> 8,
      uint16_t(incandescent_lookup.g * max_val) >> 8,
      uint16_t(incandescent_lookup.b * max_val) >> 8);
  }
}
*/

void render_bulb_cover() {
  SQ15x16 cover[4] = { 0.25, 1.00, 0.25, 0.00 };

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    CRGB16 covered_color = {
      leds_16[i].r * cover[i % 4],
      leds_16[i].g * cover[i % 4],
      leds_16[i].b * cover[i % 4],
    };

    SQ15x16 bulb_opacity = CONFIG.BULB_OPACITY;
    SQ15x16 bulb_opacity_inv = 1.0 - bulb_opacity;

    leds_16[i].r = leds_16[i].r * bulb_opacity_inv + covered_color.r * bulb_opacity;
    leds_16[i].g = leds_16[i].g * bulb_opacity_inv + covered_color.g * bulb_opacity;
    leds_16[i].b = leds_16[i].b * bulb_opacity_inv + covered_color.b * bulb_opacity;
  }
}

CRGB force_saturation(CRGB input, uint8_t saturation) {
  CHSV out_col_hsv = rgb2hsv_approximate(input);
  CHSV out_col_hsv_sat = out_col_hsv;
  out_col_hsv_sat.setHSV(out_col_hsv.h, saturation, out_col_hsv.v);

  return CRGB(out_col_hsv_sat);
}

CRGB force_hue(CRGB input, uint8_t hue) {
  CHSV out_col_hsv = rgb2hsv_approximate(input);
  CHSV out_col_hsv_hue = out_col_hsv;
  out_col_hsv_hue.setHSV(hue, out_col_hsv.s, out_col_hsv.v);

  return CRGB(out_col_hsv_hue);
}

void blend_buffers(CRGB16* output_array, CRGB16* input_a, CRGB16* input_b, uint8_t blend_mode, SQ15x16 mix) {
  if (blend_mode == BLEND_MIX) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i].r = input_a[i].r * (1.0 - mix) + input_b[i].r * (mix);
      output_array[i].g = input_a[i].g * (1.0 - mix) + input_b[i].g * (mix);
      output_array[i].b = input_a[i].b * (1.0 - mix) + input_b[i].b * (mix);
    }
  } else if (blend_mode == BLEND_ADD) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i].r = input_a[i].r + (input_b[i].r * mix);
      output_array[i].g = input_a[i].g + (input_b[i].g * mix);
      output_array[i].b = input_a[i].b + (input_b[i].b * mix);
    }
  } else if (blend_mode == BLEND_MULTIPLY) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i].r = input_a[i].r * input_b[i].r;
      output_array[i].g = input_a[i].g * input_b[i].g;
      output_array[i].b = input_a[i].b * input_b[i].b;
    }
  }
}

void apply_prism_effect(uint8_t iterations, SQ15x16 opacity) {
  for (uint8_t i = 0; i < iterations; i++) {
    memcpy(leds_16_fx, leds_16, sizeof(CRGB16) * NATIVE_RESOLUTION);

    scale_image_to_half(leds_16_fx);
    shift_leds_up(leds_16_fx, 64);
    mirror_image_downwards(leds_16_fx);

    memcpy(leds_16_fx_2, leds_16, sizeof(CRGB16) * NATIVE_RESOLUTION);
    blend_buffers(leds_16, leds_16_fx_2, leds_16_fx, BLEND_ADD, opacity);
  }
}

void clear_leds() {
  memset(leds_16, 0, sizeof(CRGB16) * 128);
}

void process_color_shift() {
  int16_t rounded_index = spectral_history_index - 1;
  while (rounded_index < 0) {
    rounded_index += SPECTRAL_HISTORY_LENGTH;
  }

  SQ15x16 novelty_now = novelty_curve[rounded_index];

  // Remove bottom 10%, stretch values to still occupy full 0.0-1.0 range
  novelty_now -= SQ15x16(0.10);
  if (novelty_now < 0.0) {
    novelty_now = 0.0;
  }
  novelty_now *= SQ15x16(1.111111);  // ---- 1.0 / (1.0 - 0.10)

  novelty_now = novelty_now * novelty_now * novelty_now;  // Square the novelty value
  //novelty_now = (novelty_now)*0.5 + (novelty_now*novelty_now)*0.5; // "half-square" it again

  if (novelty_now > 0.05) {
    novelty_now = 0.05;
  }

  if (novelty_now > hue_shift_speed) {
    hue_shift_speed = novelty_now * SQ15x16(0.75);
  } else {
    hue_shift_speed *= SQ15x16(0.99);
  }

  // Add and wrap
  hue_position += (hue_shift_speed * hue_push_direction);
  while (hue_position < 0.0) {
    hue_position += 1.0;
  }
  while (hue_position >= 1.0) {
    hue_position -= 1.0;
  }

  if (fabs_fixed(hue_position - hue_destination) <= 0.01) {
    hue_push_direction *= -1.0;
    hue_shifting_mix_target *= -1.0;
    hue_destination = random_float();
    //printf("###################################### NEW DEST: %f\n", hue_destination);
  }

  SQ15x16 hue_shifting_mix_distance = fabs_fixed(hue_shifting_mix - hue_shifting_mix_target);
  if (hue_shifting_mix < hue_shifting_mix_target) {
    hue_shifting_mix += hue_shifting_mix_distance * 0.01;
  } else if (hue_shifting_mix > hue_shifting_mix_target) {
    hue_shifting_mix -= hue_shifting_mix_distance * 0.01;
  }

  /*
    if(chance(0.2) == true){ //0.2% of loops
        hue_push_direction *= -1.0;
        printf("###################################### SWITCH DIR: %f\n", hue_push_direction);
    }
    */
}

void make_smooth_chromagram() {
  memset(chromagram_smooth, 0, sizeof(SQ15x16) * 12);

  for (uint8_t i = 0; i < CONFIG.CHROMAGRAM_RANGE; i++) {
    SQ15x16 note_magnitude = spectrogram_smooth[i];

    if (note_magnitude > 1.0) {
      note_magnitude = 1.0;
    } else if (note_magnitude < 0.0) {
      note_magnitude = 0.0;
    }

    uint8_t chroma_bin = i % 12;
    chromagram_smooth[chroma_bin] += note_magnitude / SQ15x16(CONFIG.CHROMAGRAM_RANGE / 12.0);
  }

  static SQ15x16 max_peak = 0.001;

  max_peak *= 0.999;
  if (max_peak < 0.01) {
    max_peak = 0.01;
  }

  for (uint16_t i = 0; i < 12; i++) {
    if (chromagram_smooth[i] > max_peak) {
      SQ15x16 distance = chromagram_smooth[i] - max_peak;
      max_peak += distance *= SQ15x16(0.05);
    }
  }

  SQ15x16 multiplier = 1.0 / max_peak;

  for (uint8_t i = 0; i < 12; i++) {
    chromagram_smooth[i] *= multiplier;
  }
}


void draw_sprite(CRGB16 dest[], CRGB16 sprite[], uint32_t dest_length, uint32_t sprite_length, float position, SQ15x16 alpha) {
  int32_t position_whole = position;  // Downcast to integer accuracy
  float position_fract = position - position_whole;
  SQ15x16 mix_right = position_fract;
  SQ15x16 mix_left = 1.0 - mix_right;

  for (uint16_t i = 0; i < sprite_length; i++) {
    int32_t pos_left = i + position_whole;
    int32_t pos_right = i + position_whole + 1;

    bool skip_left = false;
    bool skip_right = false;

    if (pos_left < 0) {
      pos_left = 0;
      skip_left = true;
    }
    if (pos_left > dest_length - 1) {
      pos_left = dest_length - 1;
      skip_left = true;
    }

    if (pos_right < 0) {
      pos_right = 0;
      skip_right = true;
    }
    if (pos_right > dest_length - 1) {
      pos_right = dest_length - 1;
      skip_right = true;
    }

    if (skip_left == false) {
      dest[pos_left].r += sprite[i].r * mix_left * alpha;
      dest[pos_left].g += sprite[i].g * mix_left * alpha;
      dest[pos_left].b += sprite[i].b * mix_left * alpha;
    }

    if (skip_right == false) {
      dest[pos_right].r += sprite[i].r * mix_right * alpha;
      dest[pos_right].g += sprite[i].g * mix_right * alpha;
      dest[pos_right].b += sprite[i].b * mix_right * alpha;
    }
  }
}

CRGB16 force_saturation_16(CRGB16 rgb, SQ15x16 saturation) {
  // Convert RGB to HSV
  SQ15x16 max_val = fmax_fixed(rgb.r, fmax_fixed(rgb.g, rgb.b));
  SQ15x16 min_val = fmax_fixed(rgb.r, fmax_fixed(rgb.g, rgb.b));
  SQ15x16 delta = max_val - min_val;

  SQ15x16 hue, saturation_value, value;
  value = max_val;

  if (delta == 0) {
    // The color is achromatic (gray)
    hue = 0;
    saturation_value = 0;
  } else {
    // Calculate hue and saturation
    if (max_val == rgb.r) {
      hue = (rgb.g - rgb.b) / delta;
    } else if (max_val == rgb.g) {
      hue = 2 + (rgb.b - rgb.r) / delta;
    } else {
      hue = 4 + (rgb.r - rgb.g) / delta;
    }

    hue *= 60;
    if (hue < 0) {
      hue += 360;
    }

    saturation_value = delta / max_val;
  }

  // Set the saturation to the input value
  saturation_value = saturation;

  // Convert back to RGB
  SQ15x16 c = saturation_value * value;
  SQ15x16 x = c * (1 - fabs_fixed(fmod_fixed(hue / 60, 2) - 1));
  SQ15x16 m = value - c;

  CRGB16 modified_rgb;
  if (hue >= 0 && hue < 60) {
    modified_rgb.r = c;
    modified_rgb.g = x;
    modified_rgb.b = 0;
  } else if (hue >= 60 && hue < 120) {
    modified_rgb.r = x;
    modified_rgb.g = c;
    modified_rgb.b = 0;
  } else if (hue >= 120 && hue < 180) {
    modified_rgb.r = 0;
    modified_rgb.g = c;
    modified_rgb.b = x;
  } else if (hue >= 180 && hue < 240) {
    modified_rgb.r = 0;
    modified_rgb.g = x;
    modified_rgb.b = c;
  } else if (hue >= 240 && hue < 300) {
    modified_rgb.r = x;
    modified_rgb.g = 0;
    modified_rgb.b = c;
  } else {
    modified_rgb.r = c;
    modified_rgb.g = 0;
    modified_rgb.b = x;
  }

  modified_rgb.r += m;
  modified_rgb.g += m;
  modified_rgb.b += m;

  return modified_rgb;
}

CRGB16 adjust_hue_and_saturation(CRGB16 color, SQ15x16 hue, SQ15x16 saturation) {
  // Store the RGB values
  SQ15x16 r = color.r, g = color.g, b = color.b;

  // Calculate maximum and minimum values of r, g, b
  SQ15x16 max_val = fmax_fixed(r, fmax_fixed(g, b));

  // Calculate the value of the HSV color
  SQ15x16 v = max_val;

  // Use the input saturation
  SQ15x16 s = saturation;

  // Prepare to convert HSV back to RGB
  SQ15x16 c = v * s;  // chroma
  SQ15x16 h_prime = fmod_fixed(hue * SQ15x16(6.0), SQ15x16(6.0));
  SQ15x16 x = c * (SQ15x16(1.0) - fabs_fixed(fmod_fixed(h_prime, SQ15x16(2.0)) - SQ15x16(1.0)));

  // Recalculate r, g, b based on the new hue and saturation
  if (h_prime >= 0 && h_prime < 1) {
    r = c;
    g = x;
    b = 0;
  } else if (h_prime >= 1 && h_prime < 2) {
    r = x;
    g = c;
    b = 0;
  } else if (h_prime >= 2 && h_prime < 3) {
    r = 0;
    g = c;
    b = x;
  } else if (h_prime >= 3 && h_prime < 4) {
    r = 0;
    g = x;
    b = c;
  } else if (h_prime >= 4 && h_prime < 5) {
    r = x;
    g = 0;
    b = c;
  } else if (h_prime >= 5 && h_prime < 6) {
    r = c;
    g = 0;
    b = x;
  }

  // Add the calculated difference to get the final RGB values
  SQ15x16 m = v - c;
  r += m;
  g += m;
  b += m;

  // Clamp the values between 0.0 and 1.0 to account for rounding errors
  r = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), r));
  g = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), g));
  b = fmax_fixed(SQ15x16(0.0), fmin_fixed(SQ15x16(1.0), b));

  // Return the resulting color
  CRGB16 result = { r, g, b };
  return result;
}