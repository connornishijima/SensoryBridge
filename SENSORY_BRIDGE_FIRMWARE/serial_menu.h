void init_serial(uint32_t baud_rate) {
  Serial.begin(baud_rate);
  bool timeout = false;
  uint32_t t_start = millis();
  uint32_t t_timeout = t_start + 250;

  while (!Serial && timeout == false) {
    if (millis() >= t_timeout) {
      timeout = true;  // Must not be connected to PC
      break;
    } else {
      yield();
    }
  }
}

void parse_command(char* command_buf) {
  if (strcmp(command_buf, "v") == 0 || strcmp(command_buf, "V") == 0) {

    Serial.println(FIRMWARE_VERSION);

  } else if (strcmp(command_buf, "factory_reset") == 0) {

    factory_reset();
    Serial.println("--- ! REBOOTING in 1 second to apply changes");
    delay(1000);
    ESP.restart();

  } else if (strcmp(command_buf, "get_main_unit") == 0) {

    Serial.print("IS_MAIN_UNIT: ");
    Serial.println(CONFIG.IS_MAIN_UNIT);

  }
  else if (strcmp(command_buf, "reboot") == 0) {

    Serial.println("--- ! REBOOTING in 1 second");
    delay(1000);
    ESP.restart();

  } else { // Commands with metadata are parsed here

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

    if (strcmp(command_type, "set_main_unit") == 0) { // Set if this Sensory Bridge is a Main Unit, 
      if(strcmp(command_data, "true") == 0){
        CONFIG.IS_MAIN_UNIT = true;
      }
      else if(strcmp(command_data, "false") == 0){
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

void check_serial(uint32_t t_now) {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      // Command recieved
      parse_command(command_buf);
      memset(&command_buf, sizeof(char) * 64, 0);
      command_buf_index = 0;
    } else {
      command_buf[command_buf_index] = c;
      command_buf_index++;
      if (command_buf_index > 63) {
        command_buf_index = 63;
      }
    }
  }
}