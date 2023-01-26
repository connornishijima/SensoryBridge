uint32_t timing_start = 0;
extern void run_sweet_spot();
extern void show_leds();

void reboot() {
  USBSerial.println("--- ! REBOOTING to apply changes (You may need to restart the Serial Monitor)");
  USBSerial.flush();
  for(float i = 1.0; i >= 0.0; i-=0.05){
    MASTER_BRIGHTNESS = i;
    run_sweet_spot();
    show_leds();
    delay(12); // Takes ~250ms total
  }
  FastLED.setBrightness(0);
  FastLED.show();
  ESP.restart();
}

void start_timing(char* func_name) {
  USBSerial.print(func_name);
  USBSerial.print(": ");
  USBSerial.flush();
  timing_start = micros();
}

void end_timing() {
  uint32_t timing_end = micros();
  uint32_t t_delta = timing_end - timing_start;

  USBSerial.print("DONE IN ");
  USBSerial.print(t_delta / 1000.0, 3);
  USBSerial.println(" MS");
}

void check_current_function() {
  function_hits[function_id]++;
}

static void usb_event_callback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    //arduino_usb_event_data_t * data = (arduino_usb_event_data_t*)event_data;
    switch (event_id) {
      case ARDUINO_USB_STARTED_EVENT:
        //Serial0.println("USB PLUGGED");
        break;
      case ARDUINO_USB_STOPPED_EVENT:
        //Serial0.println("USB UNPLUGGED");
        break;
      case ARDUINO_USB_SUSPEND_EVENT:
        //Serial0.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
        break;
      case ARDUINO_USB_RESUME_EVENT:
        //Serial0.println("USB RESUMED");
        break;

      default:
        break;
    }
  } else if (event_base == ARDUINO_FIRMWARE_MSC_EVENTS) {
    //arduino_firmware_msc_event_data_t * data = (arduino_firmware_msc_event_data_t*)event_data;
    switch (event_id) {
      case ARDUINO_FIRMWARE_MSC_START_EVENT:
        //Serial0.println("MSC Update Start");
        msc_update_started = true;
        break;
      case ARDUINO_FIRMWARE_MSC_WRITE_EVENT:
        //HWSerial.printf("MSC Update Write %u bytes at offset %u\n", data->write.size, data->write.offset);
        //Serial0.print(".");
        break;
      case ARDUINO_FIRMWARE_MSC_END_EVENT:
        //Serial0.printf("\nMSC Update End: %u bytes\n", data->end.size);
        break;
      case ARDUINO_FIRMWARE_MSC_ERROR_EVENT:
        //Serial0.printf("MSC Update ERROR! Progress: %u bytes\n", data->error.size);
        break;
      case ARDUINO_FIRMWARE_MSC_POWER_EVENT:
        //Serial0.printf("MSC Update Power: power: %u, start: %u, eject: %u", data->power.power_condition, data->power.start, data->power.load_eject);
        break;

      default:
        break;
    }
  }
}

void enable_usb_update_mode() {
  USB.onEvent(usb_event_callback);

  MSC_Update.onEvent(usb_event_callback);
  MSC_Update.begin();

  MASTER_BRIGHTNESS = 1.0;

  uint8_t led_index = 0;
  uint8_t sweet_index = 0;

  const uint8_t sweet_order[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };

  while (true) {
    for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
      leds[i] = CRGB(0, 0, 0);
    }

    if (msc_update_started == false) {
      leds[led_index] = CRGB(0, 0, 32);
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   sweet_order[sweet_index][0] * 512);
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, sweet_order[sweet_index][1] * 512);
      ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  sweet_order[sweet_index][2] * 512);
    }
    else {
      leds[NATIVE_RESOLUTION-1-led_index] = CRGB(0, 32, 0);
      ledcWrite(SWEET_SPOT_LEFT_CHANNEL,   sweet_order[sweet_index][2] * 4095);
      ledcWrite(SWEET_SPOT_CENTER_CHANNEL, sweet_order[sweet_index][1] * 4095);
      ledcWrite(SWEET_SPOT_RIGHT_CHANNEL,  sweet_order[sweet_index][0] * 4095);
    }


    show_leds();

    if(led_index == 0 || led_index == NATIVE_RESOLUTION/2){
      sweet_index++;
      if (sweet_index >= 3) {
        sweet_index = 0;
      }
    }

    led_index++;
    if (led_index >= NATIVE_RESOLUTION) {
      led_index = 0;      
    }
    yield();
  }
}

void init_usb() {
  USB.productName("Sensory Bridge");  // Doesn't work, not my fault
  USB.manufacturerName("Lixie Labs"); // Doesn't work, not my fault
  USB.VID(0x1209); // This works though, god damn I hate USB
  USB.PID(0xABED); // Cool, cool cool cool https://pid.codes/1209/ABED/

  USB.begin();
  USBSerial.begin();
}

void init_sweet_spot() {
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

    uint16_t note_index = i + CONFIG.NOTE_OFFSET;
    uint16_t max_index = sizeof(notes) / sizeof(float);

    if (note_index > max_index - 1) {
      note_index = max_index - 1;
    }

    frequencies[i].target_freq = notes[note_index];
    frequencies[i].block_size  = (CONFIG.MAX_BLOCK_SIZE) - ((CONFIG.MAX_BLOCK_SIZE * 0.95) * sqrt(sqrt(prog)));
    frequencies[i].block_size_recip = 1.0 / float(frequencies[i].block_size);

    frequencies[i].zone = (i / float(NUM_FREQS)) * NUM_ZONES;

    float w = 2.0 * PI * ((float)frequencies[i].target_freq / CONFIG.SAMPLE_RATE);
    float coeff = 2.0 * cos(w);
    frequencies[i].coeff_q14 = (1 << 14) * coeff;

    frequencies[i].window_mult = 2048.0 / frequencies[i].block_size;
  }
  end_timing();
}

void debug_function_timing(uint32_t t_now) {
  static uint32_t last_timing_print = t_now;

  if (t_now - last_timing_print >= 30000) {
    USBSerial.println("------------");
    for (uint8_t i = 0; i < 16; i++) {
      USBSerial.print(i);
      USBSerial.print(": ");
      USBSerial.println(function_hits[i]);

      function_hits[i] = 0;
    }

    last_timing_print = t_now;
  }
}

void set_mode_name(uint16_t index, char* mode_name) {
  uint8_t len = strlen(mode_name);
  for (uint8_t i = 0; i < len; i++) {
    mode_names[32 * index + i] = mode_name[i];
  }
}

void init_system() {
  noise_button.pin = NOISE_CAL_PIN;
  mode_button.pin = MODE_PIN;

  pinMode(noise_button.pin, INPUT_PULLUP);
  pinMode(mode_button.pin, INPUT_PULLUP);

  memcpy(&CONFIG_DEFAULTS, &CONFIG, sizeof(CONFIG)); // Copy defaults values to second CONFIG object

  set_mode_name(0, "GDFT");
  set_mode_name(1, "CHROMAGRAM");
  set_mode_name(2, "BLOOM");
  set_mode_name(3, "BLOOM (FAST)");
  set_mode_name(4, "VU");
  set_mode_name(5, "VU (DOT)");

  init_serial(SERIAL_BAUD);
  init_sweet_spot();
  init_fs();

  // NOISE and MODE held down on boot
  if (digitalRead(noise_button.pin) == LOW && digitalRead(mode_button.pin) == LOW) {
    restore_defaults();
  }

  init_leds();
  init_usb();

  // MODE held down on boot
  if (digitalRead(mode_button.pin) == LOW) {
    enable_usb_update_mode();
  }

  init_i2s();
  init_p2p();
  generate_a_weights();
  generate_window_lookup();
  generate_frequency_data();

  USBSerial.println("SYSTEM INIT COMPLETE!");

  if (CONFIG.BOOT_ANIMATION == true) {
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

  if (stream_fps == true) {
    USBSerial.print("sbs((fps=");
    USBSerial.print(FPS);
    USBSerial.println("))");
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
    if (t_now - last_setting_change >= 10000) {
      save_config();
      settings_updated = false;
    }
  }
}
