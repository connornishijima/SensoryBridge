/*----------------------------------------
  Sensory Bridge SYSTEM/DEBUG FUNCTIONS
----------------------------------------*/

void check_current_function(){
  function_hits[function_id]++;
}

void debug_function_timing(uint32_t t_now){
  static uint32_t last_timing_print = t_now;

  if(t_now - last_timing_print >= 30000){
    Serial.println("------------");
    for(uint8_t i = 0; i < 16; i++){
      Serial.print(i);
      Serial.print(": ");
      Serial.println(function_hits[i]);

      function_hits[i] = 0;
    }

    last_timing_print = t_now;
  }
}

void swap (float *a, float *b){
    float temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( float arr[], int n ){
    // Use a different seed value so that we don't get same
    // result each time we run this program
    randomSeed(analogRead(10));
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)    {
        int j = random(0, n);
        swap(&arr[i], &arr[j]);
    }
}

void init_system(){
  noise_button.pin = NOISE_CAL_PIN;
  mode_button.pin = MODE_PIN;

  pinMode(noise_button.pin, INPUT_PULLUP);
  pinMode(mode_button.pin, INPUT_PULLUP);

  init_serial(500000);

  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds_out, STRIP_LED_COUNT);  // GRB ordering is assumed
  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, 2000);

  FastLED.setBrightness(0); // Slowly fade in soon

  for (uint16_t i = 0; i < 128; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
    leds_out[i] = CRGB(0, 0, 0);
  }
  FastLED.show();

  if (!LittleFS.begin(true)) { // Format if failed
    Serial.println("LittleFS Mount Failed");
    return;
  }

  load_ambient_noise_calibration();
  load_config();

  init_i2s();
  init_p2p();
  init_sweet_spot();  

  for(uint8_t i = 0; i < 128; i++){
    hue_scatter[i] = (i / 127.0)*16; // generate linear array of 0.0-16.0
  }
  randomize(hue_scatter, 128); // randomize array to create dither pattern

  #ifdef DEBUG_MODE
    cpu_usage.attach_ms(5, check_current_function);
  #endif
}

void check_settings(uint32_t t_now) {
  if (settings_updated) {
    if (t_now - last_setting_change >= 3000) {
      save_config();
      settings_updated = false;
    }
  }
}

void log_fps(){
  static uint32_t last_frame = micros();
  uint32_t t_now = micros();

  uint32_t us_delta = t_now-last_frame;
  float FPS = 1000000.0 / float(us_delta);

  Serial.println(FPS);

  last_frame = t_now;
}