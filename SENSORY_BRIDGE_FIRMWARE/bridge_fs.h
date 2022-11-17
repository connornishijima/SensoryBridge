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

void load_config() {
  Serial.print("LITTLEFS: ");
  File file = LittleFS.open("/config.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open config.bin for reading!");
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[128];
    for (uint8_t i = 0; i < sizeof(CONFIG); i++) {
      config_buffer[i] = file.read();
    }
    memcpy(&CONFIG, config_buffer, sizeof(CONFIG));

    Serial.println("READ CONFIG SUCCESSFULLY");
  }
  file.close();
}

void save_ambient_noise_calibration() {
  Serial.print("SAVING AMBIENT_NOISE PROFILE...");
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing!");
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 128; i++) {
    float in_val = ambient_samples[i];

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
  Serial.print("LOADING AMBIENT_NOISE PROFILE...");
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading!");
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 128; i++) {
    temp.bytes[0] = file.read();
    temp.bytes[1] = file.read();
    temp.bytes[2] = file.read();
    temp.bytes[3] = file.read();

    ambient_samples[i] = temp.long_val_float;
  }

  file.close();
  Serial.println("LOAD COMPLETE");
}