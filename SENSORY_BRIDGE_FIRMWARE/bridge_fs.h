/*----------------------------------------
  Sensory Bridge FILESYSTEM ACCESS
----------------------------------------*/

void reboot() {
  Serial.println("--- ! REBOOTING in 1 second to apply changes (You may need to restart the Serial Monitor)");
  Serial.flush();
  delay(1000);
  ESP.restart();
}

void factory_reset() {
  Serial.print("Deleting config.bin: ");
  if (LittleFS.remove("/config.bin")) {
    Serial.println("file deleted");
  } else {
    Serial.println("delete failed");
  }

  Serial.print("Deleting noise_cal.bin: ");
  if (LittleFS.remove("/noise_cal.bin")) {
    Serial.println("file deleted");
  } else {
    Serial.println("delete failed");
  }

  reboot();
}

void save_config() {
  Serial.print("LITTLEFS: ");
  File file = LittleFS.open("/config.bin", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open config.bin for writing!");
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[128];
    memcpy(config_buffer, &CONFIG, sizeof(CONFIG));

    for (uint8_t i = 0; i < 128; i++) {
      file.write(config_buffer[i]);
    }

    Serial.println("WROTE CONFIG SUCCESSFULLY");
  }
  file.close();
}

void save_backup_config() {
  Serial.print("LITTLEFS: ");
  File file = LittleFS.open("/config.backup", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open config.backup for writing!");
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[128];
    memcpy(config_buffer, &CONFIG, sizeof(CONFIG));

    for (uint8_t i = 0; i < 128; i++) {
      file.write(config_buffer[i]);
    }

    Serial.println("WROTE CONFIG BACKUP SUCCESSFULLY");
  }
  file.close();
}

void load_config() {
  Serial.print("LITTLEFS: ");

  bool queue_factory_reset = false;
  File file = LittleFS.open("/config.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open config.bin for reading!");
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

    Serial.print("STORED VERSION IS: ");
    Serial.println(CONFIG_TEMP.VERSION);

    Serial.print("CURRENT VERSION IS: ");
    Serial.println(CONFIG_DEFAULTS.VERSION);

    if(CONFIG_TEMP.VERSION != CONFIG_DEFAULTS.VERSION){
      Serial.println("STORED CONFIG FILE IS OUTDATED!");
      //Serial.println("Factory resetting now to avoid potentially incompatible data!");
      //save_backup_config();
      //queue_factory_reset = true;
    }
    else{
      memcpy(&CONFIG, &CONFIG_TEMP, sizeof(CONFIG_TEMP));
      Serial.println("READ CONFIG SUCCESSFULLY");
    }
  }
  file.close();

  if(queue_factory_reset == true){
    factory_reset();    
  }
}

void save_ambient_noise_calibration() {
  Serial.print("SAVING AMBIENT_NOISE PROFILE... ");
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing!");
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
  Serial.println("SAVE COMPLETE");
}

void load_ambient_noise_calibration() {
  Serial.print("LOADING AMBIENT_NOISE PROFILE... ");
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading!");
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
  Serial.println("LOAD COMPLETE");
}

void init_fs(){
  Serial.print("INIT FILESYSTEM: ");
  Serial.println(LittleFS.begin(true) == true ? PASS : FAIL);

  load_ambient_noise_calibration();
  load_config();
}
