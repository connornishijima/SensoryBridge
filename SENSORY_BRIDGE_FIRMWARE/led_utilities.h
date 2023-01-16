void init_leds() {
  bool leds_started = false;
  if(CONFIG.LED_TYPE == LED_NEOPIXEL){
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds_out, CONFIG.LED_COUNT);  // GRB ordering is assumed
  }
  else if(CONFIG.LED_TYPE == LED_DOTSTAR){
    FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, BGR>(leds_out, CONFIG.LED_COUNT);
  }
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, 2000);

  for (uint8_t x = 0; x < 128; x++) {
    leds[x] = CRGB(0, 0, 0);
  }
  FastLED.show();

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

// Returns the linear interpolation of a floating point index in a CRGB array
// index is in the range of 0.0-1.0
CRGB lerp_led(float index, CRGB * led_array) {
  float index_f = index * 127.0;
  if (index_f > 127.0) {
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
  if (STRIP_LED_COUNT == 128) {
    memcpy(leds_out, leds, sizeof(leds));
  }
  else { // If not native resolution, use interpolation
    for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
      float progress = i / float(STRIP_LED_COUNT - 1);
      leds_out[i] = lerp_led(progress, leds);
    }
  }
  FastLED.show();
}

void blocking_flash(CRGB col){
  for(uint8_t i = 0; i < 128; i++){
    leds[i] = CRGB(0,0,0);
  }
  
  const uint8_t flash_times = 2;
  for(uint8_t f = 0; f < flash_times; f++){
    for(uint8_t i = 0+48; i < 128-48; i++){
      leds[i] = col;
    }
    show_leds();
    delay(150);

    for(uint8_t i = 0; i < 128; i++){
      leds[i] = CRGB(0,0,0);
    }
    show_leds();
    delay(150);
  }
}

void clear_all_led_buffers() {
  for (uint8_t i = 0; i < 128; i++) {
    leds[i]      = CRGB(0,0,0);
    leds_temp[i] = CRGB(0,0,0);
    leds_last[i] = CRGB(0,0,0);
    leds_aux [i] = CRGB(0,0,0);
    leds_fade[i] = CRGB(0,0,0);
  }

  for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
    leds_out[i] = CRGB(0,0,0);
  }
}

void scale_image_to_half() {
  for (uint16_t i = 0; i < 64; i++) {
    leds[i] = leds[i<<1];
  }
}

void shift_leds_up(uint16_t offset) {
  memcpy(leds_temp, leds, sizeof(leds));
  memcpy(leds + offset, leds_temp, (128 - offset) * sizeof(CRGB));
  memset(leds, 0, offset * sizeof(CRGB));
}


void shift_leds_down(uint16_t offset) {
  memcpy(leds, leds + offset, (128 - offset) * sizeof(CRGB));
  memset(leds + (128 - offset), 0, offset * sizeof(CRGB));
}

void mirror_image_upwards() {
  for (uint8_t i = 0; i < 64; i++) {
    leds_temp[i]       = leds[i];
    leds_temp[127 - i] = leds[i];
  }
  load_leds_from_temp();
}


void mirror_image_downwards() {
  for (uint8_t i = 0; i < 64; i++) {
    leds_temp[64 + i] = leds[64 + i];
    leds_temp[63 - i] = leds[64 + i];
  }
  load_leds_from_temp();
}

void update_retro_heat(uint32_t t_now_us) {
  static uint32_t t_last = 0;

  float heat_speed = 0.01400;
  float cool_speed = 0.00875;

  if (t_last == 0) {
    // do nothing on first frame
  }
  else {
    uint32_t delta_us = t_now_us - t_last;
    float time_scale = delta_us / 1000.0;

    float heat_speed_scaled = heat_speed * time_scale;
    float cool_speed_scaled = cool_speed * time_scale;

    for (uint16_t i = 0; i < NUM_FREQS; i++) {
      float heat_target_capped = retro_heat_targets[i];
      if (heat_target_capped > 1.0) {
        heat_target_capped = 1.0;
      }
      else if (heat_target_capped < 0.0) {
        heat_target_capped = 0.0;
      }

      if (heat_target_capped > retro_bulbs[i]) {
        retro_bulbs[i] += heat_speed_scaled;

        if (retro_bulbs[i] > heat_target_capped) {
          retro_bulbs[i] = heat_target_capped;
        }
      }
      else if (heat_target_capped < retro_bulbs[i]) {
        retro_bulbs[i] -= cool_speed_scaled;

        if (retro_bulbs[i] < heat_target_capped) {
          retro_bulbs[i] = heat_target_capped;
        }
      }
    }
  }

  t_last = t_now_us;
}


void render_retro_heat() {
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    float   heat_val_f = sqrtf(retro_bulbs[i]);
    uint8_t heat_index = uint8_t(heat_val_f * 125);

    CRGB heat_col = incandecsent_lookup[heat_index];
    CRGB white = CRGB(2*heat_val_f,2*heat_val_f,2*heat_val_f);

    heat_col += white;

    leds[i*2+0] = heat_col;
    leds[i*2+1] = heat_col;
  }
}
