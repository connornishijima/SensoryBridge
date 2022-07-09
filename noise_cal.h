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

void show_progress_bar(float progress, CRGB col){  
  uint16_t end_led = 128*progress;
  for(uint8_t i = 0; i < 128; i++){
    if(i <= end_led){
      leds[i+LED_OFFSET] = col;
    }
    else{
      leds[i+LED_OFFSET] = CRGB(0,0,0);
    }
  }

  FastLED.show();
  FastLED.clear();
}

void load_ambient_noise_calibration() {
  Serial.println("LOADING AMBIENT_NOISE PROFILE...");
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    Serial.println("- failed to open file for reading");
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 256; i++) {
    temp.bytes[0] = file.read();
    temp.bytes[1] = file.read();
    temp.bytes[2] = file.read();
    temp.bytes[3] = file.read();

    if (i < 8) {
      print_bits(temp.bytes[0]);
      print_bits(temp.bytes[1]);
      print_bits(temp.bytes[2]);
      print_bits(temp.bytes[3]);
      Serial.println();
    }

    fft_ambient_noise_sum[i] = temp.long_val;
  }

  file.close();
  Serial.println("LOAD COMPLETE");
}

void save_ambient_noise_calibration() {
  Serial.println("SAVING AMBIENT_NOISE PROFILE...");
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < 256; i++) {
    uint32_t in_val = fft_ambient_noise_sum[i];

    temp.long_val = in_val;

    file.write( temp.bytes[0] );
    file.write( temp.bytes[1] );
    file.write( temp.bytes[2] );
    file.write( temp.bytes[3] );

    if (i < 8) {
      print_bits(temp.bytes[0]);
      print_bits(temp.bytes[1]);
      print_bits(temp.bytes[2]);
      print_bits(temp.bytes[3]);
      Serial.println();
    }
  }

  file.close();
  Serial.println("SAVE COMPLETE");
}

void run_ambient_noise_calibration() {
  if (ambient_noise_samples_collected < AMBIENT_NOISE_SAMPLES) {
    float collection_progress = ambient_noise_samples_collected / float(AMBIENT_NOISE_SAMPLES);
    show_progress_bar(collection_progress, CRGB(0,16,0));
    Serial.println(collection_progress);
    
    for (uint16_t i = 0; i < 256; i++) {
      fft_ambient_noise_sum[i] += fft_output[i];
    }
    ambient_noise_samples_collected++;
  }
  else if (ambient_noise_samples_collected >= AMBIENT_NOISE_SAMPLES) {
    collecting_ambient_noise = false;
    Serial.println("AMBIENT NOISE COLLECTION COMPLETE.");
    for (uint16_t i = 1; i < 256; i++) {
      fft_ambient_noise_sum[i] /= float(AMBIENT_NOISE_SAMPLES / 3.0); // Cover existing noise with 2x compensation to account for variations
    }

    save_ambient_noise_calibration();
  }
}