/*----------------------------------------
  Sensory Bridge LED UTILITY FUNCTIONS
----------------------------------------*/

void change_contrast_float(float* arr, int16_t contrast);
uint8_t truncate_8_bit(float input);
float truncate_float(float input);
void shift_leds_up(uint16_t offset);
void shift_leds_down(uint16_t offset);
void save_leds_to_temp();
void load_leds_from_temp();
void save_leds_to_last();
void load_leds_from_last();
CRGB lerp_led(float index, CRGB * led_array);
void interpolate_scale_leds(float scale);
void flip_image();
void mirror_image_upwards();
void mirror_image_downwards();
void fade_edges(uint8_t width);
void run_warning_led();

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


void change_contrast_float(float* arr, int16_t contrast) {
  float factor = (259 * (contrast + 255)) / (255 * (259 - contrast));

  for (uint8_t i = 0; i < 128; i++) {
    float full_val = arr[i] * 255.0;
    float out_val = truncate_float( (factor * (full_val - 128) + 128) / 255.0 );
    arr[i] = out_val;
  }
}


uint8_t truncate_8_bit(float input) {
  if (input < 0.0) {
    input = 0.0;
  }
  if (input > 255.0) {
    input = 255.0;
  }
  return input;
}


float truncate_float(float input) {
  if (input < 0.0) {
    input = 0.0;
  }
  if (input > 1.0) {
    input = 1.0;
  }
  return input;
}


void interpolate_scale_leds(float scale) {
  scale = 1.0 / scale; // inverse multiplication of scale (2.0 -> 0.5, 4.0 -> 0.25)
  for (uint16_t i = 0; i < 128; i++) {
    float index_f = i / 127.0;
    CRGB out_val = lerp_led(index_f * scale, leds);
    leds_temp[i] = out_val;
  }
  load_leds_from_temp();
}

void scale_image_to_half() {
  for (uint16_t i = 0; i < 64; i++) {
    leds_temp[i] = leds[i*2];
  }
  load_leds_from_temp();
}


void flip_image() {
  for (uint8_t i = 0; i < 128; i++) {
    leds_temp[127 - i] = leds[i];
  }
  load_leds_from_temp();
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


void shift_leds_up(uint16_t offset) {
  for (int16_t i = 128 - 1; i >= 0; i--) {
    leds_temp[i] = leds[i - offset];
  }

  for (uint16_t i = 0; i < offset; i++) {
    leds_temp[i] = CRGB(0, 0, 0);
  }
  load_leds_from_temp();
}


void shift_leds_down(uint16_t offset) {
  for (int16_t i = 0; i < 128; i++) {
    leds_temp[i] = leds[i + offset];
  }

  for (uint16_t i = 0; i < offset; i++) {
    leds_temp[127 - i] = CRGB(0, 0, 0);
  }
  load_leds_from_temp();
}

void fade_edge(bool symmetry) {
  save_leds_to_temp();
  for (int16_t i = 0; i < 16; i++) {
    float prog = i / 15.0;

    leds_temp[127 - i].r *= prog;
    leds_temp[127 - i].g *= prog;
    leds_temp[127 - i].b *= prog;

    if (symmetry) {
      leds_temp[i].r *= prog;
      leds_temp[i].g *= prog;
      leds_temp[i].b *= prog;
    }
  }
  load_leds_from_temp();
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

void clear_all_led_buffers() {
  for (uint8_t i = 0; i < 128; i++) {
    leds[i] = CRGB(0,0,0);
    leds_temp[i] = CRGB(0,0,0);
    leds_last[i] = CRGB(0,0,0);
  }

  for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
    leds_out[i] = CRGB(0,0,0);
  }
}

void process_color_push(){
  fft_velocity *= 1.5;
  if (fft_velocity > 128.0) {
    fft_velocity = 128.0;
  }
  fft_velocity /= 128.0;
  fft_velocity *= fft_velocity;
  fft_velocity *= fft_velocity;
  fft_velocity *= fft_velocity;

  if (fft_velocity < 0.25) {
    fft_velocity = 0.00;
  }

  fft_velocity *= 8.0;

  if (fft_velocity > hue_push) {
    hue_push = fft_velocity;
  }

  if(CONFIG.IS_MAIN_UNIT || main_override){
    if(CONFIG.CHROMA >= 0.95){ // Highest 5% on the knobs sets color to automatic push
        CONFIG.BASE_HUE += hue_push;
        CONFIG.BASE_HUE += 0.025;
    }
    else{
      float hue_level = CONFIG.CHROMA * 1.0526315; // Makes lower 95% of the knob occupy 100% of the color wheel
      CONFIG.BASE_HUE = 255*hue_level;
    }
  }

  float first_pos = hue_scatter[0];
  for (uint8_t i = 0; i < 127; i++) {
    hue_scatter[i] = hue_scatter[i + 1];
  }
  hue_scatter[127] = first_pos;

  hue_push *= 0.94;
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