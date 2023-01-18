uint32_t timing_start = 0;

void start_timing(char* func_name) {
  Serial.print(func_name);
  Serial.print(": ");
  Serial.flush();
  timing_start = micros();
}

void end_timing() {
  uint32_t timing_end = micros();
  uint32_t t_delta = timing_end - timing_start;

  Serial.print("DONE IN ");
  Serial.print(t_delta / 1000.0, 3);
  Serial.println(" MS");
}

void init_sweet_spot(){
  ledcSetup(SWEET_SPOT_LEFT_CHANNEL, 500, 12);
  ledcAttachPin(SWEET_SPOT_LEFT_PIN, SWEET_SPOT_LEFT_CHANNEL);

  ledcSetup(SWEET_SPOT_CENTER_CHANNEL, 500, 12);
  ledcAttachPin(SWEET_SPOT_CENTER_PIN, SWEET_SPOT_CENTER_CHANNEL);

  ledcSetup(SWEET_SPOT_RIGHT_CHANNEL, 500, 12);
  ledcAttachPin(SWEET_SPOT_RIGHT_PIN, SWEET_SPOT_RIGHT_CHANNEL);
}

void generate_a_weights() {
  start_timing("GENERATING A-WEIGHTS");
  for (uint8_t i = 0; i < 13; i++) {
    float decibels = a_weight_table[i][1];
    float bels = decibels / 10.0;
    float ratio = pow(10, bels);
    a_weight_table[i][1] = ratio;
  }

  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    float frequency = notes[i];
    uint8_t low_index = 0;
    uint8_t high_index = 0;
    for (uint8_t x = 0; x < 13; x++) {
      float table_freq = a_weight_table[x][0];
      if (frequency >= table_freq) {
        low_index = x;
        high_index = x + 1;
      }
    }

    float low_freq = a_weight_table[low_index][0];
    float high_freq = a_weight_table[high_index][0];

    float freq_position = (frequency - low_freq) / (high_freq - low_freq);

    float interpolated_weight = (a_weight_table[low_index][1] * (1.0 - freq_position)) + (a_weight_table[high_index][1] * (freq_position));

    frequencies[i].a_weighting_ratio = interpolated_weight;
    if (frequencies[i].a_weighting_ratio > 1.0) {
      frequencies[i].a_weighting_ratio = 1.0;
    }
  }
  end_timing();
}

void generate_window_lookup() {
  start_timing("GENERATING HANN WINDOW LOOKUP TABLE");
  for (uint16_t i = 0; i < 1024; i++) {
    float ratio = i / 2047.0;
    float weighing_factor = 0.54 * (1.0 - cos(TWOPI * ratio));

    window_lookup[i]        = 32767 * weighing_factor;
    window_lookup[2047 - i] = 32767 * weighing_factor;
  }
  end_timing();
}

void generate_frequency_data() {
  start_timing("GENERATING GOERTZEL FREQUENCY ANALYSIS VARIABLES");
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    float prog = i / float(NUM_FREQS);

    uint16_t note_index = i+CONFIG.NOTE_OFFSET;
    uint16_t max_index = sizeof(notes) / sizeof(float);

    if(note_index > max_index-1){
      note_index = max_index-1;
    }

    frequencies[i].target_freq = notes[note_index];
    frequencies[i].block_size  = (CONFIG.MAX_BLOCK_SIZE) - ((CONFIG.MAX_BLOCK_SIZE*0.95) * sqrt(sqrt(prog)));
    frequencies[i].block_size_recip = 1.0 / float(frequencies[i].block_size);

    frequencies[i].zone = (i / float(NUM_FREQS)) * NUM_ZONES;
    
    float w = 2.0 * PI * ((float)frequencies[i].target_freq / CONFIG.SAMPLE_RATE);
    float coeff = 2.0 * cos(w);
    frequencies[i].coeff_q14 = (1 << 14) * coeff;

    frequencies[i].window_mult = 2048.0 / frequencies[i].block_size;
  }
  end_timing();
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

void init_system() {
  noise_button.pin = NOISE_CAL_PIN;
  mode_button.pin = MODE_PIN;

  pinMode(noise_button.pin, INPUT_PULLUP);
  pinMode(mode_button.pin, INPUT_PULLUP);
  //pinMode(I2S_DIN_PIN, INPUT);

  memcpy(&CONFIG_DEFAULTS, &CONFIG, sizeof(CONFIG)); // Copy defaults values to second CONFIG object

  init_serial(SERIAL_BAUD);
  init_sweet_spot();
  init_fs();
  init_leds();
  init_i2s();
  init_p2p();
  generate_a_weights();
  generate_window_lookup();
  generate_frequency_data();

  Serial.println("SYSTEM INIT COMPLETE!");

  if(CONFIG.BOOT_ANIMATION == true){
    intro_animation();
  }
}

void log_fps(uint32_t t_now_us) {
  static uint32_t t_last = t_now_us;
  static float fps_history[10] = {0};
  static uint8_t fps_history_index = 0;

  uint32_t frame_delta_us = t_now_us - t_last;
  float fps_now = 1000000.0 / float(frame_delta_us);

  fps_history[fps_history_index] = fps_now;

  fps_history_index++;
  if (fps_history_index >= 10) {
    fps_history_index = 0;
  }

  float fps_sum = 0;
  for (uint8_t i = 0; i < 10; i++) {
    fps_sum += fps_history[i];
  }

  FPS = fps_sum / 10.0;

  if(stream_fps == true){
    Serial.println(FPS);
  }

  t_last = t_now_us;
}

// This is to prevent overuse of internal flash writes!
// Instead of writing every single setting change to
// LittleFS, we wait until no settings have been altered
// for more than 3 seconds before attempting to update
// the flash with changes. This helps in scenarios where
// you're rapidly cycling through modes for example.
void check_settings(uint32_t t_now) {
  if (settings_updated) {
    if (t_now - last_setting_change >= 3000) {
      save_config();
      settings_updated = false;
    }
  }
}
