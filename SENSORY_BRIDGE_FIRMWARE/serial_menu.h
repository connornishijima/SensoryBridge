/*----------------------------------------
  Sensory Bridge UART COMMAND LINE
  ----------------------------------------*/

// I want this to be more fully featured in the future, but for now:
//
// These functions watch the Serial port for incoming commands,
// and perform actions based on whatis recieved.

void init_serial(uint32_t baud_rate) {
  Serial.begin(baud_rate); // Default 500,000 baud
  bool timeout = false;
  bool serial_started = true;
  uint32_t t_start = millis();
  uint32_t t_timeout = t_start + 250;

  while (!Serial && timeout == false) {
    if (millis() >= t_timeout) {
      timeout = true;  // Must not be connected to PC
      serial_started = false;
    } else {
      yield();
    }
  }

  // Print welcome message
  Serial.println("---------------------------");
  Serial.print  ("SENSORY BRIDGE | VER: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.println("---------------------------");
  Serial.println();
  Serial.print("INIT_SERIAL: ");
  Serial.println(serial_started == true ? PASS : FAIL);
}

// This is for development purposes, and allows the user to dump
// the current values of many variables to the monitor at once
void dump_info() {
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println();

  Serial.print("FIRMWARE_VERSION: ");
  Serial.println(FIRMWARE_VERSION);

  Serial.print("noise_button.pressed: ");
  Serial.println(noise_button.pressed);

  Serial.print("noise_button.last_down: ");
  Serial.println(noise_button.last_down);

  Serial.print("noise_button.last_up: ");
  Serial.println(noise_button.last_up);

  Serial.print("noise_button.pin: ");
  Serial.println(noise_button.pin);

  Serial.print("mode_button.pressed: ");
  Serial.println(mode_button.pressed);

  Serial.print("mode_button.last_down: ");
  Serial.println(mode_button.last_down);

  Serial.print("mode_button.last_up: ");
  Serial.println(mode_button.last_up);

  Serial.print("mode_button.pin: ");
  Serial.println(mode_button.pin);

  Serial.print("CONFIG.PHOTONS: ");
  Serial.println(CONFIG.PHOTONS, 3);

  Serial.print("CONFIG.CHROMA: ");
  Serial.println(CONFIG.CHROMA, 3);

  Serial.print("CONFIG.MOOD: ");
  Serial.println(CONFIG.MOOD, 3);

  Serial.print("CONFIG.BASE_HUE: ");
  Serial.println(CONFIG.BASE_HUE);

  Serial.print("CONFIG.LIGHTSHOW_MODE: ");
  Serial.println(CONFIG.LIGHTSHOW_MODE);

  Serial.print("CONFIG.MIRROR_ENABLED: ");
  Serial.println(CONFIG.MIRROR_ENABLED);

  Serial.print("CONFIG.DC_OFFSET: ");
  Serial.println(CONFIG.DC_OFFSET);

  Serial.print("CONFIG.WAVEFORM_NOISE_FLOOR: ");
  Serial.println(CONFIG.WAVEFORM_NOISE_FLOOR);

  Serial.print("CONFIG.ESPNOW_CHANNEL: ");
  Serial.println(CONFIG.ESPNOW_CHANNEL);

  Serial.print("CONFIG.IS_MAIN_UNIT: ");
  Serial.println(CONFIG.IS_MAIN_UNIT);

  Serial.print("MASTER_BRIGHTHNESS: ");
  Serial.println(MASTER_BRIGHTHNESS);

  Serial.print("noise_complete: ");
  Serial.println(noise_complete);

  Serial.print("noise_iterations: ");
  Serial.println(noise_iterations);

  Serial.print("main_override: ");
  Serial.println(main_override);

  Serial.print("last_rx_time: ");
  Serial.println(last_rx_time);

  Serial.print("last_setting_change: ");
  Serial.println(last_setting_change);

  Serial.print("settings_updated: ");
  Serial.println(settings_updated);

  Serial.print("STRIP_LED_COUNT: ");
  Serial.println(STRIP_LED_COUNT);

  Serial.print("USE_CLK_PIN: ");
  Serial.println(USE_CLK_PIN);

  Serial.print("SAMPLE_RATE: ");
  Serial.println(SAMPLE_RATE);

  Serial.print("I2S_PORT: ");
  Serial.println(I2S_PORT);

  Serial.print("FREQUENCY_HOP: ");
  Serial.println(FREQUENCY_HOP);

  Serial.print("SAMPLES_PER_CHUNK: ");
  Serial.println(SAMPLES_PER_CHUNK);

  Serial.print("SAMPLE_HISTORY_LENGTH: ");
  Serial.println(SAMPLE_HISTORY_LENGTH);

  Serial.print("i2s_read_errors: ");
  Serial.println(i2s_read_errors);

  Serial.print("AVERAGE FPS: ");
  Serial.println(FPS);

  Serial.println();
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%");
}

// This parses a completed command to decide how to handle it
void parse_command(char* command_buf) {

  // Get firmware version
  if (strcmp(command_buf, "v") == 0 || strcmp(command_buf, "V") == 0) {

    Serial.println(FIRMWARE_VERSION);

  }

  // Reset the micro
  else if (strcmp(command_buf, "reset") == 0) {

    Serial.println("RESETTING...");
    Serial.flush();
    delay(1000);
    ESP.restart();

  }

  // Clear configs and reset micro
  else if (strcmp(command_buf, "factory_reset") == 0) {

    factory_reset();
    Serial.println("--- ! REBOOTING in 1 second to apply changes");
    delay(1000);
    ESP.restart();

  }

  // Returns whether or not this is a "MAIN" unit
  else if (strcmp(command_buf, "get_main_unit") == 0) {

    Serial.print("IS_MAIN_UNIT: ");
    Serial.println(CONFIG.IS_MAIN_UNIT);

  }

  // Dump tons of variable to the monitor
  else if (strcmp(command_buf, "dump") == 0) {

    dump_info();

  }

  else { // Commands with metadata are parsed here

    // Parse command type
    char command_type[16] = { 0 };
    uint8_t reading_index = 0;
    for (uint8_t i = 0; i < 16; i++) {
      reading_index++;
      if (command_buf[i] != '=') {
        command_type[i] = command_buf[i];
      } else {
        break;
      }
    }

    // Then parse command data
    char command_data[46] = { 0 };
    for (uint8_t i = 0; i < 46; i++) {
      if (command_buf[reading_index + i] != 0) {
        command_data[i] = command_buf[reading_index + i];
      } else {
        break;
      }
    }

    // Now react accordingly:

    // Set if this Sensory Bridge is a MAIN Unit:
    if (strcmp(command_type, "set_main_unit") == 0) {
      if (strcmp(command_data, "true") == 0) {
        CONFIG.IS_MAIN_UNIT = true;
      }
      else if (strcmp(command_data, "false") == 0) {
        CONFIG.IS_MAIN_UNIT = false;
      }
      save_config();

      Serial.print("IS_MAIN_UNIT: ");
      Serial.println(CONFIG.IS_MAIN_UNIT);
      Serial.println("--- ! REBOOTING in 1 second to apply changes");
      delay(1000);
      ESP.restart();
    }
  }
}

// Called on every frame, collects incoming characters until
// potential commands are found
void check_serial(uint32_t t_now) {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n') { // If normal character, add to buffer
      command_buf[command_buf_index] = c;
      command_buf_index++;

      // Avoid overflow at the cost of breaking a long command
      if (command_buf_index > 63) { 
        command_buf_index = 63;     
      }

    } else { // If a newline character is received,
             // the command in the buffer should be parsed
      parse_command(command_buf); // Parse
      memset(&command_buf, 0, sizeof(char) * 64); // Clear
      command_buf_index = 0; // Reset
    }
  }
}
