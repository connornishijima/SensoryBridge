extern void propagate_noise_cal();
extern void start_noise_cal();

enum blending_modes {
  BLEND_MIX,
  BLEND_ADD,
  BLEND_MULTIPLY,

  NUM_BLENDING_MODES  // used to know the length of this list if it changes in the future
};

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

void apply_frame_blending() {
  uint8_t blend_val = 48;

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    //leds_frame_blending[i].r = uint16_t(leds_frame_blending[i].r * (255-blend_val)) >> 8;
    fadeToBlackBy(leds_frame_blending, NATIVE_RESOLUTION, blend_val);

    if(leds_frame_blending[i].r > leds[i].r) { leds[i].r = leds_frame_blending[i].r; }
    if(leds_frame_blending[i].g > leds[i].g) { leds[i].g = leds_frame_blending[i].g; }
    if(leds_frame_blending[i].b > leds[i].b) { leds[i].b = leds_frame_blending[i].b; }

    leds_frame_blending[i] = leds[i];
  }
}

void show_leds() {
  dither_step++;
  if (dither_step >= 8) {
    dither_step = 0;
  }

  // This is only used to fade in when booting!
  if (millis() >= 1000 && noise_transition_queued == false && mode_transition_queued == false) {
    if (MASTER_BRIGHTNESS < 1.0) {
      MASTER_BRIGHTNESS += 0.005;
    }
    if (MASTER_BRIGHTNESS > 1.0) {
      MASTER_BRIGHTNESS = 1.00;
    }
  }

  //apply_frame_blending();

  if (CONFIG.LED_COUNT == NATIVE_RESOLUTION) {
    memcpy(leds_out, leds, sizeof(leds));
  } else {  // If not native resolution, use interpolation if enabled
    if (CONFIG.LED_INTERPOLATION == true) {
      float index_push = float(1) / float(CONFIG.LED_COUNT);
      float index = 0.0;

      for (uint16_t i = 0; i < CONFIG.LED_COUNT; i++) {  // Interpolation
        leds_out[i] = lerp_led_NEW(index, leds);
        index += index_push;
      }
    } else {  // No interpolation
      float index_push = float(NATIVE_RESOLUTION) / float(CONFIG.LED_COUNT);
      float index = 0.0;

      for (uint16_t i = 0; i < CONFIG.LED_COUNT; i++) {
        leds_out[i] = leds[uint8_t(index)];
        index += index_push;
      }
    }
  }

  if (CONFIG.REVERSE_ORDER == true) {
    reverse_leds(leds_out, CONFIG.LED_COUNT);
  }

  // PHOTONS knob is squared and applied here:
  FastLED.setBrightness((255 * MASTER_BRIGHTNESS) * (CONFIG.PHOTONS * CONFIG.PHOTONS) * silent_scale);
  FastLED.setDither(CONFIG.TEMPORAL_DITHERING);
  FastLED.show();
  //USBSerial.println(MASTER_BRIGHTNESS);
}

void init_leds() {
  bool leds_started = false;

  leds_out = new CRGB[CONFIG.LED_COUNT];

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
    if (CONFIG.LED_COLOR_ORDER == RGB) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == GRB) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, GRB>(leds_out, CONFIG.LED_COUNT);
    } else if (CONFIG.LED_COLOR_ORDER == BGR) {
      FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, BGR>(leds_out, CONFIG.LED_COUNT);
    }
  }

  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, CONFIG.MAX_CURRENT_MA);

  for (uint8_t x = 0; x < NATIVE_RESOLUTION; x++) {
    leds[x] = CRGB(0, 0, 0);
  }
  show_leds();

  leds_started = true;

  USBSerial.print("INIT_LEDS: ");
  USBSerial.println(leds_started == true ? PASS : FAIL);
}

void save_leds_to_fx() {
  memcpy(leds_fx, leds, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void load_leds_from_fx() {
  memcpy(leds, leds_fx, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void save_leds_to_aux() {
  memcpy(leds_aux, leds, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void load_leds_from_aux() {
  memcpy(leds, leds_aux, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void save_leds_to_last() {
  memcpy(leds_last, leds, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void load_leds_from_last() {
  memcpy(leds, leds_last, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void save_leds_to_temp() {
  memcpy(leds_temp, leds, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void load_leds_from_temp() {
  memcpy(leds, leds_temp, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void blocking_flash(CRGB col) {
  led_thread_halt = true;
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  const uint8_t flash_times = 2;
  for (uint8_t f = 0; f < flash_times; f++) {
    for (uint8_t i = 0 + 48; i < NATIVE_RESOLUTION - 48; i++) {
      leds[i] = col;
    }
    show_leds();
    FastLED.delay(150);

    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    show_leds();
    FastLED.delay(150);
  }

  led_thread_halt = false;
}

void clear_all_led_buffers() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
    leds_temp[i] = CRGB(0, 0, 0);
    leds_last[i] = CRGB(0, 0, 0);
    leds_aux[i] = CRGB(0, 0, 0);
    leds_fade[i] = CRGB(0, 0, 0);
  }

  for (uint16_t i = 0; i < CONFIG.LED_COUNT; i++) {
    leds_out[i] = CRGB(0, 0, 0);
  }
}

void scale_image_to_half(CRGB* led_array) {
  for (uint16_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    leds_temp[i] = led_array[i << 1];
    leds_temp[64 + i] = CRGB(0, 0, 0);
  }

  memcpy(led_array, leds_temp, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void scale_half_to_full(CRGB* led_array) {
  float index_push = float(0.5) / float(NATIVE_RESOLUTION);
  float index = 0.0;

  for (uint16_t i = 0; i < NATIVE_RESOLUTION; i++) {  // Interpolation
    leds_temp[i] = lerp_led_NEW(index, led_array);
    index += index_push;
  }

  memcpy(led_array, leds_temp, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void shift_leds_up(CRGB* led_array, uint16_t offset) {
  memcpy(leds_temp, led_array, sizeof(CRGB) * NATIVE_RESOLUTION);
  memcpy(led_array + offset, leds_temp, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
  memset(led_array, 0, offset * sizeof(CRGB));
}


void shift_leds_down(CRGB* led_array, uint16_t offset) {
  memcpy(led_array, led_array + offset, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
  memset(led_array + (NATIVE_RESOLUTION - offset), 0, offset * sizeof(CRGB));
}

void mirror_image_upwards(CRGB* led_array) {
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    leds_temp[i] = led_array[i];
    leds_temp[(NATIVE_RESOLUTION - 1) - i] = led_array[i];
  }

  memcpy(led_array, leds_temp, sizeof(CRGB) * NATIVE_RESOLUTION);
}

void mirror_image_downwards(CRGB* led_array) {
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION >> 1); i++) {
    leds_temp[64 + i] = led_array[64 + i];
    leds_temp[63 - i] = led_array[64 + i];
  }

  memcpy(led_array, leds_temp, sizeof(CRGB) * NATIVE_RESOLUTION);
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
      CRGB out_col = CHSV(255 * progress, 0, 255 * led_level);
      leds[i] = out_col;
    }
    show_leds();
    FastLED.delay(3);
  }

  clear_all_led_buffers();

  const uint8_t particle_count = 16;
  struct particle {
    float phase;
    float speed;
    CRGB col;
  };
  particle particles[particle_count];

  for (uint8_t i = 0; i < particle_count; i++) {
    float prog = i / float(particle_count);
    particles[i].phase = 0.0;
    particles[i].speed = 0.002 * (i + 1);
    particles[i].col = CHSV(255 * prog, 255 * CONFIG.SATURATION, 255);
  }
  MASTER_BRIGHTNESS = 1.0;
  float center_brightness = 0.0;

  for (uint16_t i = 0; i < 50; i++) {
    if (center_brightness < 1.0) {
      center_brightness += 0.2;
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, (center_brightness * center_brightness) * 4096);
    }

    float dimming = 1.0;
    float anim_prog = i / 50.0;
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
        CRGB out_col = particles[p].col;
        out_col.r *= led_level;
        out_col.g *= led_level;
        out_col.b *= led_level;
        leds[i] += out_col;
      }
    }
    show_leds();
    FastLED.delay(6);
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

// Draws anti-aliased 1-D lines using Xiaolin Wu's algorithm onto a CRGB array
// Start and end positions are floating point for subpixel positioning
void draw_line(CRGB* led_array, CRGB line_color, float start_pos, float end_pos) {
  // Get the start and end positions as integers
  int16_t x0 = max((int)start_pos, 0);
  int16_t x1 = min((int)end_pos, NATIVE_RESOLUTION - 1);
  //float gradient = (float)(line_color.r - line_color.b) / (x1 - x0);

  // Determine whether the line is going left or right
  if (x0 > x1) {
    // If the line is going left, swap the start and end positions
    int16_t temp = x0;
    x0 = x1;
    x1 = temp;
  }

  // Iterate over the pixels of the line
  for (uint16_t x = x0; x <= x1; x++) {
    float t = (x - x0) / (float)(x1 - x0);
    //int16_t y = (int16_t)(line_color.b + gradient * (x - x0));
    // determine the coverage of the current pixel
    uint16_t coverage = (uint16_t)((1 - t) * 8);

    // Blend the line color with the background color
    CRGB blended_color = blend(line_color, led_array[x], coverage);
    led_array[x] = blended_color;
  }
}


float apply_contrast_float(float value, float intensity) {
  float mid_point = 0.5;
  float factor = (intensity * 2.0) + 1.0;

  float contrasted_value = (value - mid_point) * factor + mid_point;
  contrasted_value = constrain(contrasted_value, 0.0, 1.0);

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

void apply_incandescent_filter() {
  uint8_t mix = CONFIG.INCANDESCENT_FILTER * 255;
  uint8_t inv_mix = 255 - mix;

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    uint8_t filtered_r = uint16_t(leds[i].r * incandescent_lookup.r) >> 8;
    uint8_t filtered_g = uint16_t(leds[i].g * incandescent_lookup.g) >> 8;
    uint8_t filtered_b = uint16_t(leds[i].b * incandescent_lookup.b) >> 8;

    leds[i].r = (uint16_t(leds[i].r * inv_mix) >> 8) + (uint16_t(filtered_r * mix) >> 8);
    leds[i].g = (uint16_t(leds[i].g * inv_mix) >> 8) + (uint16_t(filtered_g * mix) >> 8);
    leds[i].b = (uint16_t(leds[i].b * inv_mix) >> 8) + (uint16_t(filtered_b * mix) >> 8);

    uint8_t max_val = 0;
    if (leds[i].r > max_val) { max_val = leds[i].r; }
    if (leds[i].g > max_val) { max_val = leds[i].g; }
    if (leds[i].b > max_val) { max_val = leds[i].b; }

    uint8_t leakage = uint16_t((max_val >> 2) * mix) >> 8;

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
  }
}

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

void render_bulb_cover() {
  const uint8_t cover[4] = { 64, 255, 64, 0 };

  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    CRGB covered_color = CRGB(
      scale8(leds[i].r, cover[i % 4]),
      scale8(leds[i].g, cover[i % 4]),
      scale8(leds[i].b, cover[i % 4]));

    leds[i] = blend(leds[i], covered_color, 255 * CONFIG.BULB_OPACITY);
  }
}

CRGB force_saturation(CRGB input, uint8_t saturation) {
  CHSV out_col_hsv = rgb2hsv_approximate(input);
  CHSV out_col_hsv_sat = out_col_hsv;
  out_col_hsv_sat.setHSV(out_col_hsv.h, saturation, out_col_hsv.v);

  return CRGB(out_col_hsv_sat);
}

void blend_buffers(CRGB* output_array, CRGB* input_a, CRGB* input_b, uint8_t blend_mode, uint8_t mix) {
  if (blend_mode == BLEND_MIX) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i] = input_a[i].scale8(255 - mix);
      input_b[i].scale8(mix);
    }
  } else if (blend_mode == BLEND_ADD) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i] = input_a[i] + input_b[i].scale8(mix);
    }
  } else if (blend_mode == BLEND_MULTIPLY) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      output_array[i].r = uint16_t(input_a[i].r * input_b[i].r) >> 8;
      output_array[i].g = uint16_t(input_a[i].g * input_b[i].g) >> 8;
      output_array[i].b = uint16_t(input_a[i].b * input_b[i].b) >> 8;
    }
  }
}

void apply_prism_effect(uint8_t iterations, uint8_t opacity) {
  for (uint8_t i = 0; i < iterations; i++) {
    save_leds_to_fx();

    scale_image_to_half(leds_fx);
    shift_leds_up(leds_fx, 64);
    mirror_image_downwards(leds_fx);

    save_leds_to_temp();
    blend_buffers(leds, leds_temp, leds_fx, BLEND_ADD, opacity);
  }
}