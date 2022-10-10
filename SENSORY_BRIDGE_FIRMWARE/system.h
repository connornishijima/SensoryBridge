#include <driver/adc.h>

void read_config();
void load_dc_offset();
void write_config();
void write_dc_offset();

void print_system_info() {
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
  Serial.println("FPS value, and use the 128th LED to     ");
  Serial.println("indicate audio clipping.                ");
  Serial.println();
  Serial.println("(Press MODE again to continue booting!)");

  Serial.flush();
}

void init_serial(uint32_t baud_rate) {
  Serial.begin(baud_rate);
  bool timeout = false;
  uint32_t t_start = millis();
  uint32_t t_timeout = t_start + 250;

  while (!Serial && timeout == false) {
    if (millis() >= t_timeout) {
      timeout = true; // Must not be connected to PC
      break;
    }
    else {
      yield();
    }
  }
}

void init_bridge() {
  // Uncomment/edit one of the following lines for your led strip type.
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds_out, STRIP_LED_COUNT);
  //FastLED.addLeds<DOTSTAR, LED_DATA_PIN, LED_CLOCK_PIN, RGB>(leds_out, STRIP_LED_COUNT);
  FastLED.setMaxPowerInVoltsAndMilliamps(5.0, 2000);
  for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
    leds_out[i] = CRGB(0, 0, 0);
  }

  FastLED.show();

  init_usb();

  if(init_wifi()){
    Serial.println("CONNECTED TO WIFI");
    discovery_check_in();
  }
  else{
    Serial.println("FAILED TO CONNECT TO WIFI");
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    last_fft_frame[i] = 0.01;
  }

  uint8_t GRADIENT_HALLOWEEN[] = {
    0,      255,  32,  0,   // Orange
    63,     255,  32,  0,   // Orange
    127,    255,  0,   155, // Hint of purple
    195,    255,  32,  0,   // Orange
    255,    255,  32,  0,   // Wrap to same orange from index 0
  };

  current_palette = RainbowColors_p; //.loadDynamicGradientPalette( GRADIENT_HALLOWEEN );

  noise_button.pin = NOISE_CAL_PIN;
  mode_button.pin = MODE_PIN;

  pinMode(noise_button.pin, INPUT_PULLUP);
  pinMode(mode_button.pin, INPUT_PULLUP);

  init_serial(500000);

  if (digitalRead(mode_button.pin) == LOW) {
    while (digitalRead(mode_button.pin) == LOW) {
      yield();
    }
    debug_mode = true;
    print_system_info();

    while (digitalRead(mode_button.pin) == HIGH) { // Wait on second MODE press
      yield();
    }
  }
  else {
    Serial.print("SBFV: ");
    Serial.println(FIRMWARE_VERSION);
    Serial.flush();
  }

  if (!LittleFS.begin(true)) { // Format if failed
    if (debug_mode) {
      Serial.println("LittleFS Mount Failed");
    }
    return;
  }

  read_config();
  load_dc_offset();

  for (uint16_t i = 0; i < 128; i++) {
    fft_ambient_noise[i] = 0;
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
  uint32_t t_now = millis();
  if (digitalRead(noise_button.pin) == LOW) {
    if (noise_button.pressed == false) {
      noise_button.pressed = true;
      noise_button.last_down = t_now;
    }

    if (t_now - noise_button.last_down > 250 && noise_button.last_up < noise_button.last_down) {
      for (uint16_t i = 0; i < 128; i++) {
        fft_ambient_noise[i] = 0;
      }
      save_dc_offset();
      save_ambient_noise_calibration();
      if (debug_mode) {
        Serial.println("CLEARED AMBIENT NOISE CALIBRATION!");
      }

      noise_button.last_up = t_now;
    }
  }
  else if (digitalRead(noise_button.pin) == HIGH) {
    if (noise_button.pressed == true) {
      noise_button.pressed = false;
      noise_button.last_up = t_now;

      uint32_t press_duration = noise_button.last_up - noise_button.last_down;

      if (press_duration < 250) {
        if (debug_mode) {
          Serial.println("COLLECTING AMBIENT NOISE SAMPLES IN 500 MS");
        }
        collecting_ambient_noise = true;
        DC_OFFSET = 0;

        ambient_noise_samples_collected = 0;
        for (uint16_t i = 0; i < 128; i++) {
          fft_ambient_noise[i] = 0;
        }

        for (uint16_t i = 0; i < 128; i++) {
          for (uint16_t x = 0; x < 128; x++) {
            leds[x] = CRGB(0, 0, 0);
          }
          uint16_t ir = (STRIP_LED_COUNT - 1) - i;
          leds[ir] = CRGB(0, 255, 255);
          show_leds();
        }

        clear_all_led_buffers();
      }
    }
  }

  if (digitalRead(mode_button.pin) == LOW) {
    if (mode_button.pressed == false) {
      mode_button.pressed = true;
      mode_button.last_down = t_now;
    }

    if (t_now - mode_button.last_down > 250 && mode_button.last_up < mode_button.last_down) {
      MIRROR_ENABLED = !MIRROR_ENABLED;
      last_setting_change = millis();
      settings_updated = true;
      mode_button.last_up = t_now;
    }
  }
  else if (digitalRead(mode_button.pin) == HIGH) {
    if (mode_button.pressed == true) {
      mode_button.pressed = false;
      mode_button.last_up = t_now;

      uint32_t press_duration = mode_button.last_up - mode_button.last_down;

      if (press_duration < 250) {
        LIGHTSHOW_MODE++;
        if (LIGHTSHOW_MODE >= NUM_MODES) {
          LIGHTSHOW_MODE = 0;
        }

        last_setting_change = millis();
        settings_updated = true;
      }
    }
  }
}

void check_settings() {
  if (settings_updated) {
    uint32_t t_now = millis();
    if (t_now - last_setting_change >= 3000) {
      write_config();
      settings_updated = false;
    }
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

    float smooth_low  = 0.600;
    float smooth_high = 0.990;

    SMOOTHING = (SMOOTHING) * (smooth_high - smooth_low) / (1.0) + smooth_low;

    float sensitivity_low  = 3.2767;
    float sensitivity_high = 3276.7;

    FFT_CEILING = (SENSITIVITY) * (sensitivity_high - sensitivity_low) / (1.0) + sensitivity_low;
    //Serial.println(FFT_CEILING);

    current_brightness = uint8_t(BRIGHTNESS * 255);
    FastLED.setBrightness(current_brightness);
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

  sweet_spot_down = fabs((SWEET_SPOT + 0.2) - multiplier_sum);
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

  sweet_spot_up = fabs((SWEET_SPOT - 0.2) - multiplier_sum);
  if (sweet_spot_up > 0.125) {
    sweet_spot_up = 0.125;
  }
  sweet_spot_up *= 8.0;
  sweet_spot_up = 1.0 - sweet_spot_up;
  sweet_spot_up = sweet_spot_up * sweet_spot_up;

  sweet_spot_down   = int16_t(sweet_spot_down   * 2048);
  sweet_spot_center = int16_t(sweet_spot_center * 2048);
  sweet_spot_up     = int16_t(sweet_spot_up     * 2048);

  if (sweet_spot_down < 10) {
    sweet_spot_down = 10;
  }
  if (sweet_spot_center < 10) {
    sweet_spot_center = 10;
  }
  if (sweet_spot_up < 10) {
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

void start_timing(String section_name) {
#ifdef LOG_TIMING
  if (section_start != 0 || section_end != 0) {
    section_end = micros();
    Serial.print(current_section);
    Serial.print(": ");
    Serial.print(section_end - section_start);
    Serial.println("us");
  }
  current_section = section_name;
  section_start = micros();
#endif
}

void end_timing() {
#ifdef LOG_TIMING
  section_end = micros();
  Serial.print(current_section);
  Serial.print(": ");
  Serial.print(section_end - section_start);
  Serial.println("us");

  section_start = 0;
  section_end   = 0;
#endif
}

void read_config() {
  File file = LittleFS.open("/config.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open config.bin for reading");
    }
    return;
  }
  else {
    file.seek(0);
    LIGHTSHOW_MODE = file.read();
    MIRROR_ENABLED = file.read();

    if (debug_mode) {
      Serial.println("READ CONFIG SUCCESSFULLY");
    }
  }
  file.close();
}

void write_config() {
  File file = LittleFS.open("/config.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open config.bin for writing");
    }
    return;
  }
  else {
    file.seek(0);
    file.write(LIGHTSHOW_MODE);
    file.write(MIRROR_ENABLED);

    if (debug_mode) {
      Serial.println("WROTE CONFIG SUCCESSFULLY");
    }
  }
  file.close();
}
