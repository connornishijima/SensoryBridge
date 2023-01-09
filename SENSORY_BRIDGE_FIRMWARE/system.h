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
  for (uint16_t i = 0; i < 128; i++) {
    float ratio = i / 255.0;
    //float weighingFactor = 0.2810639 - (0.5208972 * cos(TWOPI * ratio)) + (0.1980399 * cos(FOURPI * ratio));
    float weighingFactor = 0.54 * (1.0 - cos(TWOPI * ratio));

    window_lookup[i] = weighingFactor;
    window_lookup[255 - i] = weighingFactor;
  }
  end_timing();
}

void generate_frequency_data() {
  start_timing("GENERATING GOERTZEL FREQUENCY ANALYSIS VARIABLES");
  for (uint8_t i = 0; i < NUM_FREQS; i++) {
    float prog = i / float(NUM_FREQS);

    frequencies[i].target_freq = notes[i];
    frequencies[i].block_size  = float(SAMPLE_RATE / frequencies[i].target_freq) * (8.0 - ((i / 48.0)*1.0));
    frequencies[i].k = (0.5 + (frequencies[i].block_size * frequencies[i].target_freq) / float(SAMPLE_RATE));
    frequencies[i].w = ((2 * PI) / float(frequencies[i].block_size)) * frequencies[i].k;
    frequencies[i].cosine = cos(frequencies[i].w);
    frequencies[i].sine   = sin(frequencies[i].w);
    frequencies[i].coeff  = 2 * frequencies[i].cosine;
    frequencies[i].q0 = 0;
    frequencies[i].q1 = 0;
    frequencies[i].q2 = 0;
  }
  end_timing();
}

void check_current_function(){
  function_hits[function_id]++;
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
  pinMode(I2S_DIN_PIN, INPUT);

  init_serial(500000);
  delay(50);
  init_leds();
  delay(50);
  init_fs();
  delay(50);
  init_i2s();
  delay(50);
  init_p2p();
  delay(50);
  generate_a_weights();
  delay(50);
  generate_window_lookup();
  delay(50);
  generate_frequency_data();
  delay(50);

  #if DEBUG_MODE == 1
    Serial.println("---------- DEBUG MODE ENABLED, CPU BENCHMARKING ACTIVE");
    cpu_usage.attach_ms(5, check_current_function);
  #endif

  Serial.println("SYSTEM INIT COMPLETE!");

  for(uint8_t i = 0; i < NUM_FREQS; i++){
    float samples_per_block = frequencies[i].block_size;
    float seconds_per_block = samples_per_block / float(SAMPLE_RATE);

    Serial.print(i);
    Serial.print(":\t");
    Serial.print(frequencies[i].target_freq);
    Serial.print(":\t");
    Serial.print(samples_per_block);
    Serial.print(":\t");
    Serial.println(seconds_per_block,5);
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
