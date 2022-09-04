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


void show_leds(){
	if(!collecting_ambient_noise){
		//shift_leds_up(LED_OFFSET);
    run_warning_led();
		FastLED.show();
	}

  current_frame++;
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
  CRGB out_col = CRGB(0,0,0);
  out_col.r += (1 - index_f_frac) * led_array[index_i].r;
  out_col.g += (1 - index_f_frac) * led_array[index_i].g;
  out_col.b += (1 - index_f_frac) * led_array[index_i].b;

  out_col.r += index_f_frac * led_array[index_i + 1].r;
  out_col.g += index_f_frac * led_array[index_i + 1].g;
  out_col.b += index_f_frac * led_array[index_i + 1].b;
  return out_col;
}


void shift_leds_up(uint16_t offset) {
  for (int16_t i = NUM_LEDS - 1; i >= 0; i--) {
    leds_temp[i] = leds[i - offset];
  }

  for (uint16_t i = 0; i < offset; i++) {
    leds_temp[i] = CRGB(0, 0, 0);
  }
  load_leds_from_temp();
}


void shift_leds_down(uint16_t offset) {
  for (int16_t i = 0; i < NUM_LEDS; i++) {
    leds_temp[i] = leds[i + offset];
  }

  for (uint16_t i = 0; i < offset; i++) {
    leds_temp[127 - i] = CRGB(0, 0, 0);
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


void fade_edges(uint8_t width) {
  for (uint8_t i = 0; i < width; i++) {
    float progress = i / float(width);
    progress *= progress;

    leds[i].r *= progress;
    leds[i].g *= progress;
    leds[i].b *= progress;

    leds[127 - i].r *= progress;
    leds[127 - i].g *= progress;
    leds[127 - i].b *= progress;
  }
}


void process_color_push(){
	float velocity_sum = 0.0;
	for(uint16_t i = 0; i < 128; i++){
		float velocity = fft_velocities[i];
		if(velocity < 0.25){
			velocity = 0.0;
		}
		velocity_sum += velocity;
	}

	float push_velocity = velocity_sum / 128.0;
	if(push_velocity > 1.0){
		push_velocity = 1.0;
	}
	
	push_velocity *= push_velocity;
	push_velocity *= push_velocity;
	push_velocity *= push_velocity;
	push_velocity *= 32.0;

  push_velocity *= (((1.0-SMOOTHING)*0.75) + 0.25);

  //Serial.println(push_velocity);

	if(push_velocity > hue_push){
		hue_push = push_velocity;
	}
	else{
		hue_push *= 0.98;
	}

	if (hue_push > 10.0) {
		hue_push = 10.0;
	}

 if(hue_push < 0.1){
   hue_push = 0.1;
 }
 
	hue -= hue_push;
}

void run_warning_led(){
  if(warn == true){
    leds[NUM_LEDS-1] = CRGB(255,0,0);
    warn = false;
  }
  else{
    leds[NUM_LEDS-1] = CRGB(0,0,0);    
  }
}
