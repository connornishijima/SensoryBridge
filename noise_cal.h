void print_bits(uint8_t input) {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print( bitRead(input, 7 - i) );
  }
}

void print_bits(uint32_t input) {
  for (uint8_t i = 0; i < 32; i++) {
    Serial.print( bitRead(input, 31 - i) );
  }
}

void show_progress_bar(float progress, CRGB col) {
  for (uint8_t i = 0; i < 128; i++) {
    leds[i] = CRGB(0,0,0);
  }
  uint16_t end_led = 128 * progress;
  float max_val = 0.0;
  for (uint8_t i = 0; i < 128; i++) {
    if (fft_ambient_noise[i] > max_val) {
      max_val = fft_ambient_noise[i];
    }
  }
  for (uint8_t i = 0; i < 128; i++) {
    if (i == end_led) {
      leds[i] = col;
    }
    else if(i < end_led){
      float out_val = (fft_ambient_noise[i] / max_val);
      // out_val = out_val * out_val;
      leds[i] = CHSV(16,255,255 * out_val);
    }
  }

  show_leds();
}

void load_ambient_noise_calibration() {
  if (debug_mode) {
    Serial.println("LOADING AMBIENT_NOISE PROFILE...");
  }
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open file for reading");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 128; i++) {
    temp.bytes[0] = file.read();
    temp.bytes[1] = file.read();
    temp.bytes[2] = file.read();
    temp.bytes[3] = file.read();

    if (debug_mode) {
      if (i < 8) {
        print_bits(temp.bytes[0]);
        print_bits(temp.bytes[1]);
        print_bits(temp.bytes[2]);
        print_bits(temp.bytes[3]);
        Serial.println();
      }
    }

    fft_ambient_noise[i] = temp.long_val;
  }

  file.close();
  if (debug_mode) {
    Serial.println("LOAD COMPLETE");
  }
}

void load_dc_offset() {
  if (debug_mode) {
    Serial.println("LOADING DC OFFSET...");
  }
  File file = LittleFS.open("/dc_offset.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open file for reading");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  temp.bytes[0] = file.read();
  temp.bytes[1] = file.read();
  temp.bytes[2] = file.read();
  temp.bytes[3] = file.read();

  DC_OFFSET = temp.long_val_signed;

  file.close();
  if (debug_mode) {
    Serial.println("LOAD COMPLETE");
  }
}

void save_ambient_noise_calibration() {
  if (debug_mode) {
    Serial.println("SAVING AMBIENT_NOISE PROFILE...");
  }
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open file for writing");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 128; i++) {
    uint32_t in_val = fft_ambient_noise[i];

    temp.long_val = in_val;

    file.write( temp.bytes[0] );
    file.write( temp.bytes[1] );
    file.write( temp.bytes[2] );
    file.write( temp.bytes[3] );

    if (debug_mode) {
      if (i < 8) {
        print_bits(temp.bytes[0]);
        print_bits(temp.bytes[1]);
        print_bits(temp.bytes[2]);
        print_bits(temp.bytes[3]);
        Serial.println();
      }
    }
  }

  file.close();
  if (debug_mode) {
    Serial.println("SAVE COMPLETE");
  }
}

void save_dc_offset() {
  if (debug_mode) {
    Serial.println("SAVING DC OFFSET...");
  }
  File file = LittleFS.open("/dc_offset.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      Serial.println("- failed to open file for writing");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  temp.long_val_signed = DC_OFFSET;

  file.write( temp.bytes[0] );
  file.write( temp.bytes[1] );
  file.write( temp.bytes[2] );
  file.write( temp.bytes[3] );

  file.close();
  if (debug_mode) {
    Serial.println("SAVE COMPLETE");
  }
}

void run_ambient_noise_calibration() {
  if (ambient_noise_samples_collected < AMBIENT_NOISE_SAMPLES) {
    float collection_progress = ambient_noise_samples_collected / float(AMBIENT_NOISE_SAMPLES);
    show_progress_bar(collection_progress, CRGB(0, 255, 255));

    for (uint16_t i = 0; i < 128; i++) {
      if (fft_integer[i] > fft_ambient_noise[i]) {
        fft_ambient_noise[i] = fft_integer[i];
      }
    }
    ambient_noise_samples_collected++;
  }
  else if (ambient_noise_samples_collected >= AMBIENT_NOISE_SAMPLES) {
    collecting_ambient_noise = false;

    DC_OFFSET /= float(AMBIENT_NOISE_SAMPLES);

    Serial.print("DC OFFSET: ");
    Serial.println(DC_OFFSET);

    if (debug_mode) {
      Serial.println("AMBIENT NOISE COLLECTION COMPLETE.");
    }

    save_ambient_noise_calibration();
    save_dc_offset();
  }
}
