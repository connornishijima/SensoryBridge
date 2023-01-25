/*----------------------------------------
  Sensory Bridge FILESYSTEM ACCESS
  ----------------------------------------*/

extern void reboot(); // system.h

// Restore all defaults defined in globals.h by removing saved data and rebooting
void factory_reset() {
  USBSerial.print("Deleting config.bin: ");
  if (LittleFS.remove("/config.bin")) {
    USBSerial.println("file deleted");
  } else {
    USBSerial.println("delete failed");
  }

  USBSerial.print("Deleting noise_cal.bin: ");
  if (LittleFS.remove("/noise_cal.bin")) {
    USBSerial.println("file deleted");
  } else {
    USBSerial.println("delete failed");
  }

  reboot();
}

// Restore only configuration defaults
void restore_defaults() {
  USBSerial.print("Deleting config.bin: ");
  if (LittleFS.remove("/config.bin")) {
    USBSerial.println("file deleted");
  } else {
    USBSerial.println("delete failed");
  }

  reboot();
}

// Save configuration to LittleFS
void save_config() {
  if (debug_mode) {
    USBSerial.print("LITTLEFS: ");
  }
  File file = LittleFS.open("/config.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      USBSerial.println("Failed to open config.bin for writing!");
    }
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[128];
    memcpy(config_buffer, &CONFIG, sizeof(CONFIG));

    for (uint8_t i = 0; i < 128; i++) {
      file.write(config_buffer[i]);
    }

    if (debug_mode) {
      USBSerial.println("WROTE CONFIG SUCCESSFULLY");
    }
  }
  file.close();
}

// Save configuration to LittleFS 10 seconds from now
void save_config_delayed() {
  last_setting_change = millis();
  settings_updated = true;
}

// Load configuration from LittleFS
void load_config() {
  if (debug_mode) {
    USBSerial.print("LITTLEFS: ");
  }

  bool queue_factory_reset = false;
  File file = LittleFS.open("/config.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      USBSerial.println("Failed to open config.bin for reading!");
    }
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[256];
    for (uint8_t i = 0; i < sizeof(CONFIG); i++) {
      config_buffer[i] = file.read();
    }

    // Temporary buffer is used to compare saved config version with this firmware versions' defaults.
    // If a leftover config file version is a mismatch with the current firmware version, the config
    // and noise_cal files need to be factory reset, to avoid reading potentially corrupt values
    conf CONFIG_TEMP;
    memcpy(&CONFIG_TEMP, config_buffer, sizeof(CONFIG_TEMP));

    if (debug_mode) {
      USBSerial.print("STORED VERSION IS: ");
      USBSerial.println(CONFIG_TEMP.VERSION);

      USBSerial.print("CURRENT VERSION IS: ");
      USBSerial.println(CONFIG_DEFAULTS.VERSION);
    }

    if (CONFIG_TEMP.VERSION != CONFIG_DEFAULTS.VERSION) {
      if(debug_mode){USBSerial.println("STORED CONFIG FILE IS OUTDATED!");}
      //USBSerial.println("Factory resetting now to avoid potentially incompatible data!");
      //save_backup_config();
      //queue_factory_reset = true;
    }
    else {
      memcpy(&CONFIG, &CONFIG_TEMP, sizeof(CONFIG_TEMP));
      if(debug_mode){USBSerial.println("READ CONFIG SUCCESSFULLY");}
    }
  }
  file.close();

  if (queue_factory_reset == true) {
    factory_reset();
  }
}

// Save noise calibration to LittleFS
void save_ambient_noise_calibration() {
  if(debug_mode){USBSerial.print("SAVING AMBIENT_NOISE PROFILE... ");}
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    if(debug_mode){USBSerial.println("Failed to open file for writing!");}
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    float in_val = noise_samples[i];

    temp.long_val_float = in_val;

    file.write(temp.bytes[0]);
    file.write(temp.bytes[1]);
    file.write(temp.bytes[2]);
    file.write(temp.bytes[3]);
  }

  file.close();
  if(debug_mode){USBSerial.println("SAVE COMPLETE");}
}

// Load noise calibration from LittleFS
void load_ambient_noise_calibration() {
  if(debug_mode){USBSerial.print("LOADING AMBIENT_NOISE PROFILE... ");}
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    if(debug_mode){USBSerial.println("Failed to open file for reading!");}
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    temp.bytes[0] = file.read();
    temp.bytes[1] = file.read();
    temp.bytes[2] = file.read();
    temp.bytes[3] = file.read();

    noise_samples[i] = temp.long_val_float;
  }

  file.close();
  if(debug_mode){USBSerial.println("LOAD COMPLETE");}
}

// Initialize LittleFS
void init_fs() {
  USBSerial.print("INIT FILESYSTEM: ");
  USBSerial.println(LittleFS.begin(true) == true ? PASS : FAIL);

  load_ambient_noise_calibration();
  load_config();
}
