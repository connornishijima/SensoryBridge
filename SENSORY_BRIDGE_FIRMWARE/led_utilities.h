extern void propagate_noise_cal();
extern void start_noise_cal();

void run_sweet_spot() {
  static float sweet_spot_brightness = 0.0; // init to zero for first fade in

  if (sweet_spot_brightness < 1.0) {
    sweet_spot_brightness += 0.05;
  }

  if (sweet_spot_state > sweet_spot_state_follower) {
    float delta = sweet_spot_state - sweet_spot_state_follower;
    sweet_spot_state_follower += delta * 0.05;
  }
  else if (sweet_spot_state < sweet_spot_state_follower) {
    float delta = sweet_spot_state_follower - sweet_spot_state;
    sweet_spot_state_follower -= delta * 0.05;
  }

  uint16_t led_power[3] = {0, 0, 0};
  for (float i = -1; i <= 1; i++) {
    float position_delta = fabs(i - sweet_spot_state_follower);
    if (position_delta > 1.0) {
      position_delta = 1.0;
    }

    float led_level = 1.0 - position_delta;
    led_level *= led_level;
    //                                                Never fully dim
    led_power[uint8_t(i + 1)] = 4095 * led_level * (0.1+silent_scale*0.9) * sweet_spot_brightness * CONFIG.PHOTONS;
  }

  ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   led_power[0]);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, led_power[1]);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  led_power[2]);
}

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0-1.0
CRGB lerp_led(float index, CRGB* led_array) {
  float NUM_LEDS_NATIVE = NATIVE_RESOLUTION-1; // count from zero
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

void show_leds() {
  if (STRIP_LED_COUNT == NATIVE_RESOLUTION) {
    memcpy(leds_out, leds, sizeof(leds));
  } else {  // If not native resolution, use interpolation
    for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
      float progress = i / float(STRIP_LED_COUNT - 1);
      leds_out[i] = lerp_led(progress, leds);
    }
  }
  // PHOTONS knob is squared and applied here:
  FastLED.setBrightness((255 * MASTER_BRIGHTNESS) * (CONFIG.PHOTONS * CONFIG.PHOTONS) * silent_scale);
  FastLED.show();
}

void init_leds() {
  bool leds_started = false;
  if (CONFIG.LED_TYPE == LED_NEOPIXEL) {
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds_out, CONFIG.LED_COUNT);  // GRB ordering is assumed
  } else if (CONFIG.LED_TYPE == LED_DOTSTAR) {
    FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, BGR>(leds_out, CONFIG.LED_COUNT);
  }

  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, 2000);

  for (uint8_t x = 0; x < NATIVE_RESOLUTION; x++) {
    leds[x] = CRGB(0, 0, 0);
  }
  show_leds();

  leds_started = true;

  Serial.print("INIT_LEDS: ");
  Serial.println(leds_started == true ? PASS : FAIL);
}

void save_leds_to_last() {
  memcpy(leds_last, leds, sizeof(leds));
}


void load_leds_from_last() {
  memcpy(leds, leds_last, sizeof(leds));
}


void save_leds_to_temp() {
  memcpy(leds_temp, leds, sizeof(leds));
}


void load_leds_from_temp() {
  memcpy(leds, leds_temp, sizeof(leds));
}

void blocking_flash(CRGB col) {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  const uint8_t flash_times = 2;
  for (uint8_t f = 0; f < flash_times; f++) {
    for (uint8_t i = 0 + 48; i < NATIVE_RESOLUTION - 48; i++) {
      leds[i] = col;
    }
    show_leds();
    delay(150);

    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    show_leds();
    delay(150);
  }
}

void clear_all_led_buffers() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    leds[i] = CRGB(0, 0, 0);
    leds_temp[i] = CRGB(0, 0, 0);
    leds_last[i] = CRGB(0, 0, 0);
    leds_aux[i] = CRGB(0, 0, 0);
    leds_fade[i] = CRGB(0, 0, 0);
  }

  for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
    leds_out[i] = CRGB(0, 0, 0);
  }
}

void scale_image_to_half() {
  for (uint16_t i = 0; i < (NATIVE_RESOLUTION>>1); i++) {
    leds[i] = leds[i << 1];
  }
}

void shift_leds_up(uint16_t offset) {
  memcpy(leds_temp, leds, sizeof(leds));
  memcpy(leds + offset, leds_temp, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
  memset(leds, 0, offset * sizeof(CRGB));
}


void shift_leds_down(uint16_t offset) {
  memcpy(leds, leds + offset, (NATIVE_RESOLUTION - offset) * sizeof(CRGB));
  memset(leds + (NATIVE_RESOLUTION - offset), 0, offset * sizeof(CRGB));
}

void mirror_image_upwards() {
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION>>1); i++) {
    leds_temp[i] = leds[i];
    leds_temp[(NATIVE_RESOLUTION-1) - i] = leds[i];
  }
  load_leds_from_temp();
}

void mirror_image_downwards() {
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION>>1); i++) {
    leds_temp[64 + i] = leds[64 + i];
    leds_temp[63 - i] = leds[64 + i];
  }
  load_leds_from_temp();
}

void intro_animation() {
  MASTER_BRIGHTNESS = 1.0;
  ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   0.0 * 4096);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, 0.0 * 4096);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  0.0 * 4096);

  for (float progress = 0.3; progress <= 0.925; progress += 0.01) {
    float total_vals = 0.925 - 0.3;
    float brightness = (progress - 0.3) / total_vals;

    brightness *= brightness;

    MASTER_BRIGHTNESS = brightness;
    ledcWrite(SWEET_SPOT_LEFT_CHANNEL,  brightness * 4096);
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
    delay(4);
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
    particles[i].col = CHSV(255 * prog, 255, 255);
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
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   0);
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, dimming * 4096);
      ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  0);
    }
    else {
      anim_prog *= 2.0;
      dimming = 1.0 - anim_prog;
      dimming *= dimming;
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL,  dimming * 4096);
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
    delay(5);
  }
  MASTER_BRIGHTNESS = 0.0;
  ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   0);
  ledcWrite(SWEET_SPOT_CENTER_CHANNEL, 0);
  ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  0);
}

void run_transition_fade() {
  if (MASTER_BRIGHTNESS > 0.0) {
    MASTER_BRIGHTNESS -= 0.05;

    if (MASTER_BRIGHTNESS < 0.0) {
      MASTER_BRIGHTNESS = 0.0;
    }
  }
  else {
    if (mode_transition_queued == true) { // If transition for MODE button press
      mode_transition_queued = false;
      CONFIG.LIGHTSHOW_MODE++;
      if (CONFIG.LIGHTSHOW_MODE >= NUM_MODES) {
        CONFIG.LIGHTSHOW_MODE = 0;
      }
    }
    if (noise_transition_queued == true) { // If transition for NOISE button press
      noise_transition_queued = false;
      // start noise cal
      Serial.println("COLLECTING AMBIENT NOISE SAMPLES...");
      propagate_noise_cal();
      start_noise_cal();
    }
  }
}

void distort_exponential() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION-1);
    float prog_distorted = prog * prog;
    leds_temp[i] = lerp_led(prog_distorted, leds);
  }
  load_leds_from_temp();
}

void distort_logarithmic() {
  for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
    float prog = i / float(NATIVE_RESOLUTION-1);
    float prog_distorted = sqrt(prog);
    leds_temp[i] = lerp_led(prog_distorted, leds);
  }
  load_leds_from_temp();
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
  for (uint8_t i = 0; i < (NATIVE_RESOLUTION>>1); i++) {
    float fade = i / float(NATIVE_RESOLUTION>>1);

    leds[(NATIVE_RESOLUTION - 1 - i) + shift].r *= fade;
    leds[(NATIVE_RESOLUTION - 1 - i) + shift].g *= fade;
    leds[(NATIVE_RESOLUTION - 1 - i) + shift].b *= fade;
  }
}
