#include <driver/adc.h>

void print_system_info(){
  Serial.print("\n\n\n");

  Serial.println("--------------------------");
  Serial.println("* SENSORY BRIDGE *");
  Serial.println("--------------------------");
  
  Serial.print("FIRMWARE VERSION: ");
  Serial.println(FIRMWARE_VERSION);

  Serial.println("--------------------------");

  Serial.println("Welcome to Sensory Bridge!");
  Serial.println("By holding down the MODE button on boot,");
  Serial.println("you've now enabled debug mode.");
  Serial.println();
  Serial.println("Currently, this has no effect other than");
  Serial.println("to print the firmware version number and");
  Serial.println("FPS value.");
  Serial.println();
  Serial.println("(Press MODE again to continue booting!)");

  Serial.flush();
}

void init_serial(uint32_t baud_rate){
  Serial.begin(baud_rate);
  bool timeout = false;
  uint32_t t_start = millis();
  uint32_t t_timeout = t_start + 250;

  while(!Serial && timeout == false){
    if(millis() >= t_timeout){
      timeout = true; // Must not be connected to PC
      break;
    }
    else{
      yield();
    }
  }
}

void init_bridge() {
  pinMode(NOISE_CAL_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);

  init_serial(500000);

  if(digitalRead(MODE_PIN) == LOW){
    while(digitalRead(MODE_PIN) == LOW){
      yield();
    }
    debug_mode = true;
    print_system_info();

    while(digitalRead(MODE_PIN) == HIGH){ // Wait on second MODE press
      yield();
    }
  }
  else{
    Serial.print("SBFV: ");
    Serial.println(FIRMWARE_VERSION);
    Serial.flush();
  }

  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  // FastLED.addLeds<SM16703, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1829, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1803, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903B, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1904, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS2903, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2852, LED_DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2812B, LED_DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<GS1903, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SK6812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<SK6822, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA106, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<PL9823, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SK6822, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2813, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811_400, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GE8822, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205_400, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD1886, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
  // ## Clocked (SPI) types ##
  // FastLED.addLeds<LPD6803, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<LPD8806, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2801, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2803, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
  // FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
  // FastLED.addLeds<APA102, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
  // FastLED.addLeds<SK9822, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical

  //FastLED.setMaxPowerInVoltsAndMilliamps(5.0, 4000);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    last_fft_frame[i] = 0.01;
  }
  FastLED.show();
  delay(500);

  for (uint16_t i = 0; i < 128; i++) {
    fft_ambient_noise[i] = 0;
  }

  if (!LittleFS.begin(true)) { // Format if failed
    if(debug_mode){
      Serial.println("LittleFS Mount Failed");
    }
    return;
  }

  load_ambient_noise_calibration();

  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
  adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_11db);

  ledcSetup(0, 500, 12);
  ledcAttachPin(SWEET_SPOT_LEFT_PIN, 0);

  ledcSetup(1, 500, 12);
  ledcAttachPin(SWEET_SPOT_CENTER_PIN, 1);

  ledcSetup(2, 500, 12);
  ledcAttachPin(SWEET_SPOT_RIGHT_PIN, 2);

  INIT_I2S();
}

void log_fps() {
  static uint32_t last_call = micros();
  uint32_t t_now = micros();

  uint32_t t_delta = t_now - last_call;
  FPS = 1000000.0 / float(t_delta);

  Serial.print("FPS: ");
  Serial.println(FPS);

  last_call = t_now;
}

void check_buttons() {
  // NOISE CAL ------------------------------------------
  bool noise_cal_reset = false;
  if (digitalRead(NOISE_CAL_PIN) == LOW) {
    uint32_t click_start = millis();
    while (digitalRead(NOISE_CAL_PIN) == LOW) {
      uint32_t t_now = millis();
      if (t_now - click_start >= 1000) {
        noise_cal_reset = true;
        break;
      }
      delay(1);
    }

    if (!noise_cal_reset) {
      if(debug_mode){
        Serial.println("COLLECTING AMBIENT NOISE SAMPLES IN 500 MS");
      }
      collecting_ambient_noise = true;
      ambient_noise_samples_collected = 0;
      for (uint16_t i = 0; i < 128; i++) {
        fft_ambient_noise[i] = 0;
      }

      delay(500);
    }
    else { // Noise cal reset
      for (uint16_t i = 0; i < 128; i++) {
        fft_ambient_noise[i] = 0;
      }
      save_ambient_noise_calibration();
      if(debug_mode){
        Serial.println("CLEARED AMBIENT NOISE CALIBRATION!");
      }
    }

    while (digitalRead(NOISE_CAL_PIN) == LOW) {
      delay(1);
    }
  }

  // MODE SELECT ----------------------------------------
  bool mode_pressed = false;
  bool mode_long_press = false;
  if (digitalRead(MODE_PIN) == LOW) {
    mode_pressed = true;
    uint32_t t_start = millis();
    while (digitalRead(MODE_PIN) == LOW) {
      yield();
    }
    uint32_t t_end = millis();
    if((t_end-t_start) >= 250){
      mode_long_press = true;
    }
  }

  if(mode_pressed && !mode_long_press){
    LIGHTSHOW_MODE++;
    if (LIGHTSHOW_MODE >= NUM_MODES) {
      LIGHTSHOW_MODE = 0;
    }
  }
  else if(mode_pressed && mode_long_press){
    incandescent_mode = !incandescent_mode;
  }
}

void check_knobs() {
  static uint32_t iter = 0;
  const uint8_t averaging_steps  = 4;
  static float  last_brightness  = 0.0;
  static float  last_smoothing   = 1.0;
  static float  last_sensitivity = 0.0;

  iter++;

  if (iter % 10 == 0) {
    float brightness_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      brightness_sum += (analogRead(PHOTONS_PIN) / 8191.0);
    }
    BRIGHTNESS = (1.0 - brightness_sum / float(averaging_steps)) * 1.25;

    float smoothing_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      smoothing_sum += (1.0 - (analogRead(MOOD_PIN) / 8191.0));
    }
    SMOOTHING = 1.0 - smoothing_sum / float(averaging_steps);

    float sensitivity_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      sensitivity_sum += (analogRead(BOOST_PIN) / 8191.0);
    }
    float SENSITIVITY = sensitivity_sum / float(averaging_steps);

    if (BRIGHTNESS < 0.0) {
      BRIGHTNESS = 0.0;
    }
    else if (BRIGHTNESS > 1.0) {
      BRIGHTNESS = 1.0;
    }

    BRIGHTNESS = BRIGHTNESS * BRIGHTNESS;

    BRIGHTNESS = (last_brightness * 0.75) + (BRIGHTNESS * 0.25);
    last_brightness = BRIGHTNESS;

    SMOOTHING = (last_smoothing * 0.9) + (SMOOTHING * 0.1);
    last_smoothing = SMOOTHING;

    SENSITIVITY = (last_sensitivity * 0.9) + (SENSITIVITY * 0.1);
    last_sensitivity = SENSITIVITY;

    //SENSITIVITY = 1.0 - SENSITIVITY;

    float smooth_low  = 0.700;
    float smooth_high = 0.950;

    SMOOTHING = (SMOOTHING) * (smooth_high - smooth_low) / (1.0) + smooth_low;

    float sensitivity_low  = 327.67;
    float sensitivity_high = 32767;

    FFT_CEILING = (SENSITIVITY) * (sensitivity_high - sensitivity_low) / (1.0) + sensitivity_low;
    //Serial.println(FFT_CEILING);

    FastLED.setBrightness(uint8_t(BRIGHTNESS * 255));
  }
  /*
    Serial.print(BRIGHTNESS,6);
    Serial.print("\t");
    Serial.println(SMOOTHING,6);
  */
}

void check_sweet_spot() {
  //Serial.println(multiplier_sum);
  float sweet_spot_down   = 0.0;
  float sweet_spot_center = 0.0;
  float sweet_spot_up     = 0.0;

  sweet_spot_down = fabs((SWEET_SPOT+0.2) - multiplier_sum);
  if (sweet_spot_down > 0.125) {
    sweet_spot_down = 0.125;
  }
  sweet_spot_down *= 8.0;
  sweet_spot_down = 1.0 - sweet_spot_down;
  sweet_spot_down = sweet_spot_down * sweet_spot_down;

  sweet_spot_center = fabs(SWEET_SPOT - multiplier_sum);
  if (sweet_spot_center > 0.125) {
    sweet_spot_center = 0.125;
  }
  sweet_spot_center *= 8.0;
  sweet_spot_center = 1.0 - sweet_spot_center;
  sweet_spot_center = sweet_spot_center * sweet_spot_center;

  sweet_spot_up = fabs((SWEET_SPOT-0.2) - multiplier_sum);
  if (sweet_spot_up > 0.125) {
    sweet_spot_up = 0.125;
  }
  sweet_spot_up *= 8.0;
  sweet_spot_up = 1.0 - sweet_spot_up;
  sweet_spot_up = sweet_spot_up * sweet_spot_up;

  sweet_spot_down   = int16_t(sweet_spot_down   * 2048);
  sweet_spot_center = int16_t(sweet_spot_center * 1024); // dimmer than others to appear same brightness with different LED type
  sweet_spot_up     = int16_t(sweet_spot_up     * 2048);

  if(sweet_spot_down < 10){
    sweet_spot_down = 10;
  }
  if(sweet_spot_center < 10){
    sweet_spot_center = 10;
  }
  if(sweet_spot_up < 10){
    sweet_spot_up = 10;
  }

  ledcWrite(0, sweet_spot_down  );
  ledcWrite(1, sweet_spot_center);
  ledcWrite(2, sweet_spot_up    );
}

void check_serial() {
  char command_buf[64];
  uint8_t command_buf_index = 0;
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n') {
      command_buf[command_buf_index] = c;
    }
    else { // end of command
      command_buf[command_buf_index] = 0;

      if (strcmp(command_buf, "NOISE_CAL") == 0) {
        Serial.println("COLLECTING AMBIENT NOISE SAMPLES IN 1000 MS");
        collecting_ambient_noise = true;
        ambient_noise_samples_collected = 0;
        for (uint16_t i = 0; i < 128; i++) {
          fft_ambient_noise[i] = 0;
        }

        delay(1000);
      }
      else if (strcmp(command_buf, "NOISE_CAL_RESET") == 0) {
        for (uint16_t i = 0; i < 128; i++) {
          fft_ambient_noise[i] = 0;
        }
        save_ambient_noise_calibration();
        Serial.println("CLEARED AMBIENT NOISE CALIBRATION!");
      }
      else if (strcmp(command_buf, "FIRMWARE_VERSION") == 0) {
        Serial.print("#SBFWV|");
        Serial.println(FIRMWARE_VERSION);
      }
    }

    command_buf_index++;
    if (command_buf_index >= 64) {
      command_buf_index = 0;
    }
  }
}

void start_timing(String section_name){
  #ifdef LOG_TIMING
  if(section_start != 0 || section_end != 0){
    section_end = micros();
    Serial.print(current_section);
    Serial.print(": ");
    Serial.print(section_end-section_start);
    Serial.println("us");    
  }
  current_section = section_name;
  section_start = micros();
  #endif
}

void end_timing(){
  #ifdef LOG_TIMING
  section_end = micros();
  Serial.print(current_section);
  Serial.print(": ");
  Serial.print(section_end-section_start);
  Serial.println("us");

  section_start = 0;
  section_end   = 0;
  #endif
}
