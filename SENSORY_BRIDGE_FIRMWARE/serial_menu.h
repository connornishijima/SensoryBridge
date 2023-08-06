/*----------------------------------------
  Sensory Bridge UART COMMAND LINE
  ----------------------------------------*/

// These functions watch the Serial port for incoming commands,
// and perform actions based on whatis recieved.

extern void check_current_function();  // system.h
extern void reboot();                  // system.h

void tx_begin(bool error = false) {
  if (error == false) {
    USBSerial.println("sbr{{");
  } else {
    USBSerial.println("sberr[[");
  }
}

void tx_end(bool error = false) {
  if (error == false) {
    USBSerial.println("}}");
  } else {
    USBSerial.println("]]");
  }
}

void ack() {
  USBSerial.println("SBOK");
}

void bad_command(char* command_type, char* command_data) {
  tx_begin(true);
  USBSerial.print("Bad command: ");
  USBSerial.print(command_type);
  if (command_data[0] != 0) {
    USBSerial.print("=");
    USBSerial.print(command_data);
  }

  USBSerial.println();
  tx_end(true);
}

void stop_streams() {
  stream_audio = false;
  stream_fps = false;
  stream_max_mags = false;
  stream_max_mags_followers = false;
  stream_magnitudes = false;
  stream_spectrogram = false;
  stream_chromagram = false;
}

void init_serial(uint32_t baud_rate) {
  USBSerial.begin(baud_rate);  // Default 500,000 baud
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
  USBSerial.println("---------------------------");
  USBSerial.print("SENSORY BRIDGE | VER: ");
  USBSerial.println(FIRMWARE_VERSION);
  USBSerial.println("---------------------------");
  USBSerial.println();
  USBSerial.print("INIT_SERIAL: ");
  USBSerial.println(serial_started == true ? PASS : FAIL);
}

// This is for development purposes, and allows the user to dump
// the current values of many variables to the monitor at once
void dump_info() {
  USBSerial.print("FIRMWARE_VERSION: ");
  USBSerial.println(FIRMWARE_VERSION);

  USBSerial.print("CHIP ID: ");
  print_chip_id();

  USBSerial.print("noise_button.pressed: ");
  USBSerial.println(noise_button.pressed);

  USBSerial.print("noise_button.last_down: ");
  USBSerial.println(noise_button.last_down);

  USBSerial.print("noise_button.last_up: ");
  USBSerial.println(noise_button.last_up);

  USBSerial.print("noise_button.pin: ");
  USBSerial.println(noise_button.pin);

  USBSerial.print("mode_button.pressed: ");
  USBSerial.println(mode_button.pressed);

  USBSerial.print("mode_button.last_down: ");
  USBSerial.println(mode_button.last_down);

  USBSerial.print("mode_button.last_up: ");
  USBSerial.println(mode_button.last_up);

  USBSerial.print("mode_button.pin: ");
  USBSerial.println(mode_button.pin);

  USBSerial.print("CONFIG.PHOTONS: ");
  USBSerial.println(CONFIG.PHOTONS, 6);

  USBSerial.print("CONFIG.CHROMA: ");
  USBSerial.println(CONFIG.CHROMA, 6);

  USBSerial.print("CONFIG.MOOD: ");
  USBSerial.println(CONFIG.MOOD, 6);

  USBSerial.print("CONFIG.LIGHTSHOW_MODE: ");
  USBSerial.println(CONFIG.LIGHTSHOW_MODE);

  USBSerial.print("CONFIG.MIRROR_ENABLED: ");
  USBSerial.println(CONFIG.MIRROR_ENABLED);

  USBSerial.print("CONFIG.CHROMAGRAM_RANGE: ");
  USBSerial.println(CONFIG.CHROMAGRAM_RANGE);

  USBSerial.print("CONFIG.SAMPLE_RATE: ");
  USBSerial.println(CONFIG.SAMPLE_RATE);

  USBSerial.print("CONFIG.NOTE_OFFSET: ");
  USBSerial.println(CONFIG.NOTE_OFFSET);

  USBSerial.print("CONFIG.SQUARE_ITER: ");
  USBSerial.println(CONFIG.SQUARE_ITER);

  USBSerial.print("CONFIG.LED_TYPE: ");
  USBSerial.println(CONFIG.LED_TYPE);

  USBSerial.print("CONFIG.LED_COUNT: ");
  USBSerial.println(CONFIG.LED_COUNT);

  USBSerial.print("CONFIG.LED_COLOR_ORDER: ");
  USBSerial.println(CONFIG.LED_COLOR_ORDER);

  USBSerial.print("CONFIG.SAMPLES_PER_CHUNK: ");
  USBSerial.println(CONFIG.SAMPLES_PER_CHUNK);

  USBSerial.print("CONFIG.SENSITIVITY: ");
  USBSerial.println(CONFIG.SENSITIVITY, 6);

  USBSerial.print("CONFIG.BOOT_ANIMATION: ");
  USBSerial.println(CONFIG.BOOT_ANIMATION);

  USBSerial.print("CONFIG.SWEET_SPOT_MIN_LEVEL: ");
  USBSerial.println(CONFIG.SWEET_SPOT_MIN_LEVEL);

  USBSerial.print("CONFIG.SWEET_SPOT_MAX_LEVEL: ");
  USBSerial.println(CONFIG.SWEET_SPOT_MAX_LEVEL);

  USBSerial.print("CONFIG.DC_OFFSET: ");
  USBSerial.println(CONFIG.DC_OFFSET);

  USBSerial.print("CONFIG.STANDBY_DIMMING: ");
  USBSerial.println(CONFIG.STANDBY_DIMMING);

  USBSerial.print("CONFIG.REVERSE_ORDER: ");
  USBSerial.println(CONFIG.REVERSE_ORDER);

  USBSerial.print("CONFIG.IS_MAIN_UNIT: ");
  USBSerial.println(CONFIG.IS_MAIN_UNIT);

  USBSerial.print("CONFIG.MAX_CURRENT_MA: ");
  USBSerial.println(CONFIG.MAX_CURRENT_MA);

  USBSerial.print("CONFIG.TEMPORAL_DITHERING: ");
  USBSerial.println(CONFIG.TEMPORAL_DITHERING);

  USBSerial.print("CONFIG.AUTO_COLOR_SHIFT: ");
  USBSerial.println(CONFIG.AUTO_COLOR_SHIFT);

  USBSerial.print("CONFIG.INCANDESCENT_FILTER: ");
  USBSerial.println(CONFIG.INCANDESCENT_FILTER);

  USBSerial.print("CONFIG.INCANDESCENT_MODE: ");
  USBSerial.println(CONFIG.INCANDESCENT_MODE);

  USBSerial.print("CONFIG.BULB_OPACITY: ");
  USBSerial.println(CONFIG.BULB_OPACITY);

  USBSerial.print("CONFIG.SATURATION: ");
  USBSerial.println(CONFIG.SATURATION);

  USBSerial.print("CONFIG.PRISM_COUNT: ");
  USBSerial.println(CONFIG.PRISM_COUNT);

  USBSerial.print("CONFIG.BASE_COAT: ");
  USBSerial.println(CONFIG.BASE_COAT);

  USBSerial.print("CONFIG.LUMOS_ORDER: ");
  USBSerial.println(CONFIG.LUMOS_ORDER);

  USBSerial.print("MASTER_BRIGHTNESS: ");
  USBSerial.println(MASTER_BRIGHTNESS);

  USBSerial.print("stream_audio: ");
  USBSerial.println(stream_audio);

  USBSerial.print("stream_fps: ");
  USBSerial.println(stream_fps);

  USBSerial.print("stream_max_mags: ");
  USBSerial.println(stream_max_mags);

  USBSerial.print("stream_max_mags_followers: ");
  USBSerial.println(stream_max_mags_followers);

  USBSerial.print("stream_magnitudes: ");
  USBSerial.println(stream_magnitudes);

  USBSerial.print("stream_spectrogram: ");
  USBSerial.println(stream_spectrogram);

  USBSerial.print("stream_chromagram: ");
  USBSerial.println(stream_chromagram);

  USBSerial.print("debug_mode: ");
  USBSerial.println(debug_mode);

  USBSerial.print("noise_complete: ");
  USBSerial.println(noise_complete);

  USBSerial.print("noise_iterations: ");
  USBSerial.println(noise_iterations);

  USBSerial.print("main_override: ");
  USBSerial.println(main_override);

  USBSerial.print("last_rx_time: ");
  USBSerial.println(last_rx_time);

  USBSerial.print("next_save_time: ");
  USBSerial.println(next_save_time);

  USBSerial.print("settings_updated: ");
  USBSerial.println(settings_updated);

  USBSerial.print("I2S_PORT: ");
  USBSerial.println(I2S_PORT);

  USBSerial.print("SAMPLE_HISTORY_LENGTH: ");
  USBSerial.println(SAMPLE_HISTORY_LENGTH);

  USBSerial.print("silence: ");
  USBSerial.println(silence);

  USBSerial.print("mode_destination: ");
  USBSerial.println(mode_destination);

  USBSerial.print("SYSTEM_FPS: ");
  USBSerial.println(SYSTEM_FPS);

  USBSerial.print("LED_FPS: ");
  USBSerial.println(LED_FPS);
}

// This parses a completed command to decide how to handle it
void parse_command(char* command_buf) {
  uint32_t t_now_us = micros();        // Timestamp for this loop, used by some core functions
  uint32_t t_now = t_now_us / 1000.0;  // Millisecond version

  // COMMANDS WITHOUT METADATA ###############################

  // Get firmware version -----------------------------------
  if (strcmp(command_buf, "v") == 0 || strcmp(command_buf, "V") == 0 || strcmp(command_buf, "version") == 0) {

    tx_begin();
    USBSerial.print("VERSION: ");
    USBSerial.println(FIRMWARE_VERSION);
    tx_end();

  }

  // Print help ---------------------------------------------
  else if (strcmp(command_buf, "h") == 0 || strcmp(command_buf, "H") == 0 || strcmp(command_buf, "help") == 0) {

    tx_begin();
    USBSerial.println("SENSORY BRIDGE - Serial Menu ------------------------------------------------------------------------------------");
    USBSerial.println();
    USBSerial.println("                                           v | Print firmware version number");
    USBSerial.println("                                       reset | Reboot Sensory Bridge");
    USBSerial.println("                               factory_reset | Delete configuration, including noise cal, reboot");
    USBSerial.println("                            restore_defaults | Delete configuration, reboot");
    USBSerial.println("                               get_main_unit | Print if this unit is set to MAIN for SensorySync");
    USBSerial.println("                                        dump | Print tons of useful variables in realtime");
    USBSerial.println("                                        stop | Stops the output of any enabled streams");
    USBSerial.println("                                         fps | Return the system FPS");
    USBSerial.println("                                     led_fps | Return the LED FPS");
    USBSerial.println("                                     chip_id | Return the chip id (MAC) of the CPU");
    USBSerial.println("                                    get_mode | Get lightshow mode's ID (index)");
    USBSerial.println("                               get_num_modes | Return the number of modes available");
    USBSerial.println("                             start_noise_cal | Remotely begin a noise calibration");
    USBSerial.println("                             clear_noise_cal | Remotely clear the stored noise calibration");
		USBSerial.println("                               get_noise_cal | Return the stored noise calibration");
    USBSerial.println("                                    identify | Flashes the LEDs twice in yellow");
    USBSerial.println("                              set_mode=[int] | Set the mode number");
    USBSerial.println("             columnwize=[true/false/default] | Set the LED order to columns first (only for LumosStick)");
    USBSerial.println("         mirror_enabled=[true/false/default] | Remotely toggle lightshow mirroring");
    USBSerial.println("          reverse_order=[true/false/default] | Toggle whether image is flipped upside down before final rendering");
    USBSerial.println("                         get_mode_name=[int] | Get a mode's name by ID (index)");
    USBSerial.println("                               stream=[type] | Stream live data to a Serial Plotter.");
    USBSerial.println("                                               Options are: audio, fps, magnitudes, spectrogram, chromagram");
    USBSerial.println("             led_type=['neopixel'/'dotstar'] | Sets which LED protocol to use, 3 wire or 4 wire");
    USBSerial.println("                led_count=[int or 'default'] | Sets how many LEDs your display will use (native resolution is 128)");
    USBSerial.println("       led_color_order=[GRB/RGB/BGR/default] | Sets LED color ordering, default GRB");
    USBSerial.println("      led_interpolation=[true/false/default] | Toggles linear LED interpolation when running in a non-native resolution (slower)");
    USBSerial.println("                          debug=[true/false] | Enables debug mode, where functions are timed");
    USBSerial.println("               sample_rate=[hz or 'default'] | Sets the microphone sample rate");
    USBSerial.println("             note_offset=[0-32 or 'default'] | Sets the lowest note, as a positive offset from A1 (55.0Hz)");
    USBSerial.println("              square_iter=[int or 'default'] | Sets the number of times the LED output is squared (contrast)");
    USBSerial.println("        samples_per_chunk=[int or 'default'] | Sets the number of samples collected every frame");
    USBSerial.println("            sensitivity=[float or 'default'] | Sets the scaling of audio data (>1.0 is more sensitive, <1.0 is less sensitive)");
    USBSerial.println("         boot_animation=[true/false/default] | Enable or disable the boot animation");
    USBSerial.println("                  set_main_unit=[true/false] | Sets if this unit is MAIN or not for SensorySync");
    USBSerial.println("           sweet_spot_min=[int or 'default'] | Sets the minimum amplitude to be inside the 'Sweet Spot'");
    USBSerial.println("           sweet_spot_max=[int or 'default'] | Sets the maximum amplitude to be inside the 'Sweet Spot'");
		USBSerial.println("                photons=[float or 'default'] | Sets the photons knob value  (0.0 .. 1.0) aka brightness");
		USBSerial.println("                   mood=[float or 'default'] | Sets the mood knob value (0.0 .. 1.0) aka smoothing");
    USBSerial.println("        chromagram_range=[1-64 or 'default'] | Range between 1 and 64, how many notes at the bottom of the");
    USBSerial.println("                                               spectrogram should be considered in chromagram sums");
    USBSerial.println("        standby_dimming=[true/false/default] | Toggle dimming during detected silence");
    USBSerial.println("                      bass_mode=[true/false] | Toggle bass-mode, which alters note_offset and chromagram_range for bass-y tunes");
    USBSerial.println("           max_current_ma=[int or 'default'] | Sets the maximum current FastLED will attempt to limit the LED consumption to");
    USBSerial.println("     temporal_dithering=[true/false/default] | Toggle per-LED temporal dithering that simulates higher bit-depths");
    USBSerial.println("       auto_color_shift=[true/false/default] | Toggle automated color shifting based on positive spectral changes");
    USBSerial.println("    incandescent_filter=[float or 'default'] | Set the intensity of the incandescent LUT (reduces harsh blues)");
    USBSerial.println("      incandescent_mode=[true/false/default] | Force all output into monochrome and tint with 2700K incandescent color");
    USBSerial.println("              base_coat=[true/false/default] | Enable a dim gray backdrop to the LEDs (approves appearance in most modes)");
    USBSerial.println("           bulb_opacity=[float or 'default'] | Set opacity of a filter that portrays the output as 32 \"bulbs\" with separation and hot spots");
    USBSerial.println("             saturation=[float or 'default'] | Sets the saturation of internal hues");
    USBSerial.println("              prism_count=[int or 'default'] | Sets the number of times the \"prism\" effect is applied");
    USBSerial.println("                        preset=[preset_name] | Sets multiple configuration options at once to match a preset theme");
    tx_end();
  }

  // So that software can automatically identify this device -
  else if (strcmp(command_buf, "SB?") == 0) {

    USBSerial.println("SB!");

  }

  // Reset the micro ----------------------------------------
  else if (strcmp(command_buf, "reset") == 0) {

    ack();
    reboot();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "factory_reset") == 0) {

    ack();
    factory_reset();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "restore_defaults") == 0) {

    ack();
    restore_defaults();

  }

  // Return chip ID -----------------------------------------
  else if (strcmp(command_buf, "chip_id") == 0) {

    tx_begin();
    print_chip_id();
    tx_end();

  }

  // Identify unit via 2 yellow flashes ---------------------
  else if (strcmp(command_buf, "identify") == 0) {

    ack();
    CRGB16 col = {1.00, 0.25, 0.00};
    blocking_flash(col);

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "start_noise_cal") == 0) {

    ack();
    noise_transition_queued = true;

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "clear_noise_cal") == 0) {

    ack();
    clear_noise_cal();

  }

	// return configs -----------------------------------------
	else if (strcmp(command_buf, "get_noise_cal") == 0) {

		ack();
		for (uint16_t i = 0; i < NUM_FREQS; i++) {
    		USBSerial.printf("Freq %6.1f = %8.4f\n", notes[i], float(noise_samples[i]));
  		}

	}

  // Returns the number of modes available ------------------
  else if (strcmp(command_buf, "get_num_modes") == 0) {

    tx_begin();
    USBSerial.print("NUM_MODES: ");
    USBSerial.println(NUM_MODES);
    tx_end();

  }

  // Returns the mode ID ------------------------------------
  else if (strcmp(command_buf, "get_mode") == 0) {

    tx_begin();
    USBSerial.print("MODE: ");
    USBSerial.println(CONFIG.LIGHTSHOW_MODE);
    tx_end();

  }

  // Returns whether or not this is a "MAIN" unit -----------
  else if (strcmp(command_buf, "get_main_unit") == 0) {

    tx_begin();
    USBSerial.print("CONFIG.IS_MAIN_UNIT: ");
    USBSerial.println(CONFIG.IS_MAIN_UNIT);
    tx_end();

  }

  // Returns the reason why the ESP32 last rebooted ---------
  else if (strcmp(command_buf, "reset_reason") == 0) {
    tx_begin();
    switch (esp_reset_reason()) {
      case ESP_RST_UNKNOWN:
        USBSerial.println("UNKNOWN");
        break;
      case ESP_RST_POWERON:
        USBSerial.println("POWERON");
        break;
      case ESP_RST_EXT:
        USBSerial.println("EXTERNAL");
        break;
      case ESP_RST_SW:
        USBSerial.println("SOFTWARE");
        break;
      case ESP_RST_PANIC:
        USBSerial.println("PANIC");
        break;
      case ESP_RST_INT_WDT:
        USBSerial.println("INTERNAL WATCHDOG");
        break;
      case ESP_RST_TASK_WDT:
        USBSerial.println("TASK WATCHDOG");
        break;
      case ESP_RST_WDT:
        USBSerial.println("WATCHDOG");
        break;
      case ESP_RST_DEEPSLEEP:
        USBSerial.println("DEEPSLEEP");
        break;
      case ESP_RST_BROWNOUT:
        USBSerial.println("BROWNOUT");
        break;
      case ESP_RST_SDIO:
        USBSerial.println("SDIO");
        break;
    }
    tx_end();
  }

  // Dump tons of variables to the monitor ------------------
  else if (strcmp(command_buf, "dump") == 0) {

    tx_begin();
    dump_info();
    tx_end();

  }

  // If a streaming or plotting a variable, stop ------------
  else if (strcmp(command_buf, "stop") == 0) {

    stop_streams();
    ack();

  }

  // Print the average FPS ----------------------------------
  else if (strcmp(command_buf, "fps") == 0) {

    tx_begin();
    USBSerial.print("SYSTEM_FPS: ");
    USBSerial.println(SYSTEM_FPS);
    tx_end();

  }

  // Print the average FPS ----------------------------------
  else if (strcmp(command_buf, "led_fps") == 0) {

    tx_begin();
    USBSerial.print("LED_FPS: ");
    USBSerial.println(LED_FPS);
    tx_end();

  }

  // Print the knob values ----------------------------------
  else if (strcmp(command_buf, "get_knobs") == 0) {

    USBSerial.print("{");

    USBSerial.print('"');
    USBSerial.print("PHOTONS");
    USBSerial.print('"');
    USBSerial.print(':');
    USBSerial.print(CONFIG.PHOTONS);

    USBSerial.print(',');

    USBSerial.print('"');
    USBSerial.print("CHROMA");
    USBSerial.print('"');
    USBSerial.print(':');
    USBSerial.print(CONFIG.CHROMA);

    USBSerial.print(',');

    USBSerial.print('"');
    USBSerial.print("MOOD");
    USBSerial.print('"');
    USBSerial.print(':');
    USBSerial.print(CONFIG.MOOD);

    USBSerial.println('}');

  }

  // Print the button values --------------------------------
  else if (strcmp(command_buf, "get_buttons") == 0) {

    USBSerial.print("{");

    USBSerial.print('"');
    USBSerial.print("NOISE");
    USBSerial.print('"');
    USBSerial.print(':');
    USBSerial.print(digitalRead(noise_button.pin));

    USBSerial.print(',');

    USBSerial.print('"');
    USBSerial.print("MODE");
    USBSerial.print('"');
    USBSerial.print(':');
    USBSerial.print(digitalRead(mode_button.pin));

    USBSerial.println('}');

  }

  // COMMANDS WITH METADATA ##################################

  else {  // Commands with metadata are parsed here

    // PARSER #############################
    // Parse command type
    char command_type[32] = { 0 };
    uint8_t reading_index = 0;
    for (uint8_t i = 0; i < 32; i++) {
      reading_index++;
      if (command_buf[i] != '=') {
        command_type[i] = command_buf[i];
      } else {
        break;
      }
    }

    // Then parse command data
    char command_data[94] = { 0 };
    for (uint8_t i = 0; i < 94; i++) {
      if (command_buf[reading_index + i] != 0) {
        command_data[i] = command_buf[reading_index + i];
      } else {
        break;
      }
    }
    // PARSER #############################

    // Now react accordingly:

    // Set if this Sensory Bridge is a MAIN Unit --------------
    if (strcmp(command_type, "set_main_unit") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        good = true;
        CONFIG.IS_MAIN_UNIT = true;
      } else if (strcmp(command_data, "false") == 0) {
        good = true;
        CONFIG.IS_MAIN_UNIT = false;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();

        tx_begin();
        USBSerial.print("CONFIG.IS_MAIN_UNIT: ");
        USBSerial.println(CONFIG.IS_MAIN_UNIT);
        tx_end();

        reboot();
      }
    }

    // Toggle Debug Mode --------------------------------------
    else if (strcmp(command_type, "debug") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        good = true;
        debug_mode = true;
        cpu_usage.attach_ms(5, check_current_function);
      } else if (strcmp(command_data, "false") == 0) {
        good = true;
        debug_mode = false;
        cpu_usage.detach();
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        tx_begin();
        USBSerial.print("debug_mode: ");
        USBSerial.println(debug_mode);
        tx_end();
      }
    }

    // Set Sample Rate ----------------------------------------
    else if (strcmp(command_type, "sample_rate") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        good = true;
        CONFIG.SAMPLE_RATE = CONFIG_DEFAULTS.SAMPLE_RATE;
      } else {
        good = true;
        CONFIG.SAMPLE_RATE = constrain(atol(command_data), 6400, 44100);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.print("CONFIG.SAMPLE_RATE: ");
        USBSerial.println(CONFIG.SAMPLE_RATE);
        tx_end();
        reboot();
      }
    }

    // Set Mode Number ----------------------------------------
    else if (strcmp(command_type, "set_mode") == 0) {
      mode_transition_queued = true;
      mode_destination = constrain(atol(command_data), 0, NUM_MODES - 1);

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.LIGHTSHOW_MODE: ");
      USBSerial.println(mode_destination);
      tx_end();
    }

    // Set Columnwize (for LumosStick only) ----------------------
    else if (strcmp(command_type, "columnwize") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LUMOS_ORDER = CONFIG_DEFAULTS.LUMOS_ORDER;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.LUMOS_ORDER = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.LUMOS_ORDER = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.LUMOS_ORDER: ");
        USBSerial.println(CONFIG.LUMOS_ORDER);
        tx_end();
      }
    }

    // Get Mode Name By ID ------------------------------------
    else if (strcmp(command_type, "get_mode_name") == 0) {
      uint16_t mode_id = atol(command_data);

      if (mode_id < NUM_MODES) {
        char buf[32] = { 0 };
        for (uint8_t i = 0; i < 32; i++) {
          char c = mode_names[32 * mode_id + i];
          if (c != 0) {
            buf[i] = c;
          } else {
            break;
          }
        }

        tx_begin();
        USBSerial.print("MODE_NAME: ");
        USBSerial.println(buf);
        tx_end();
      } else {
        bad_command(command_type, command_data);
      }
    }

    // Set Note Offset ----------------------------------------
    else if (strcmp(command_type, "note_offset") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
      } else {
        CONFIG.NOTE_OFFSET = constrain(atol(command_data), 0, 32);
      }
      save_config();
      tx_begin();
      USBSerial.print("CONFIG.NOTE_OFFSET: ");
      USBSerial.println(CONFIG.NOTE_OFFSET);
      tx_end();
      reboot();
    }

    // Set Square Iterations ----------------------------------
    else if (strcmp(command_type, "square_iter") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SQUARE_ITER = CONFIG_DEFAULTS.SQUARE_ITER;
      } else {
        CONFIG.SQUARE_ITER = constrain(atol(command_data), 0, 10);
      }
      save_config_delayed();

      tx_begin();
      USBSerial.print("CONFIG.SQUARE_ITER: ");
      USBSerial.println(CONFIG.SQUARE_ITER);
      tx_end();
    }

    // Set LED Type ---------------------------------------
    else if (strcmp(command_type, "led_type") == 0) {
      bool good = false;
      if (strcmp(command_data, "neopixel") == 0) {
        CONFIG.LED_TYPE = LED_NEOPIXEL;
        CONFIG.LED_COLOR_ORDER = GRB;
        good = true;
      } else if (strcmp(command_data, "dotstar") == 0) {
        CONFIG.LED_TYPE = LED_DOTSTAR;
        CONFIG.LED_COLOR_ORDER = BGR;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.print("CONFIG.LED_TYPE: ");
        USBSerial.println(CONFIG.LED_TYPE);
        tx_end();
        reboot();
      }
    }

    // Set LED Count ------------------------------------
    else if (strcmp(command_type, "led_count") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COUNT = CONFIG_DEFAULTS.LED_COUNT;
      } else {
        CONFIG.LED_COUNT = constrain(atol(command_data), 1, 10000);
      }

      save_config();
      tx_begin();
      USBSerial.print("CONFIG.LED_COUNT: ");
      USBSerial.println(CONFIG.LED_COUNT);
      tx_end();
      reboot();
    }

    // Set LED Interpolation ----------------------------
    else if (strcmp(command_type, "led_interpolation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_INTERPOLATION = CONFIG_DEFAULTS.LED_INTERPOLATION;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.LED_INTERPOLATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.LED_INTERPOLATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.LED_INTERPOLATION: ");
        USBSerial.println(CONFIG.LED_INTERPOLATION);
        tx_end();
      }
    }

    // Set Base Coat ----------------------------
    else if (strcmp(command_type, "base_coat") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BASE_COAT = CONFIG_DEFAULTS.BASE_COAT;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.BASE_COAT = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.BASE_COAT = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.BASE_COAT: ");
        USBSerial.println(CONFIG.BASE_COAT);
        tx_end();
      }
    }

    // Set LED Temporal Dithering ----------------------------
    else if (strcmp(command_type, "temporal_dithering") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.TEMPORAL_DITHERING = CONFIG_DEFAULTS.TEMPORAL_DITHERING;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.TEMPORAL_DITHERING = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.TEMPORAL_DITHERING = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.TEMPORAL_DITHERING: ");
        USBSerial.println(CONFIG.TEMPORAL_DITHERING);
        tx_end();
      }
    }

    // Set LED Color Order ----------------------------
    else if (strcmp(command_type, "led_color_order") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COLOR_ORDER = CONFIG_DEFAULTS.LED_COLOR_ORDER;
        good = true;
      } else if (strcmp(command_data, "GRB") == 0) {
        CONFIG.LED_COLOR_ORDER = GRB;
        good = true;
      } else if (strcmp(command_data, "RGB") == 0) {
        CONFIG.LED_COLOR_ORDER = RGB;
        good = true;
      } else if (strcmp(command_data, "BGR") == 0) {
        CONFIG.LED_COLOR_ORDER = BGR;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.print("CONFIG.LED_COLOR_ORDER: ");
        USBSerial.println(CONFIG.LED_COLOR_ORDER);
        tx_end();
        reboot();
      }
    }

    // Set Samples Per Chunk ---------------------------
    else if (strcmp(command_type, "samples_per_chunk") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SAMPLES_PER_CHUNK = CONFIG_DEFAULTS.SAMPLES_PER_CHUNK;
      } else {
        CONFIG.SAMPLES_PER_CHUNK = constrain(atol(command_data), 0, SAMPLE_HISTORY_LENGTH);
      }

      save_config();
      tx_begin();
      USBSerial.print("CONFIG.SAMPLES_PER_CHUNK: ");
      USBSerial.println(CONFIG.SAMPLES_PER_CHUNK);
      tx_end();
      reboot();
    }

    // Set Audio Sensitivity ----------------------------
    else if (strcmp(command_type, "sensitivity") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SENSITIVITY = CONFIG_DEFAULTS.SENSITIVITY;
      } else {
        CONFIG.SENSITIVITY = atof(command_data);
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.SENSITIVITY: ");
      USBSerial.println(CONFIG.SENSITIVITY);
      tx_end();
    }

    // Toggle Boot Animation --------------------------
    else if (strcmp(command_type, "boot_animation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BOOT_ANIMATION = CONFIG_DEFAULTS.BOOT_ANIMATION;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.BOOT_ANIMATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.BOOT_ANIMATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.print("CONFIG.BOOT_ANIMATION: ");
        USBSerial.println(CONFIG.BOOT_ANIMATION);
        tx_end();
        reboot();
      }
    }

    // Toggle Lightshow Mirroring ---------------------
    else if (strcmp(command_type, "mirror_enabled") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MIRROR_ENABLED = CONFIG_DEFAULTS.MIRROR_ENABLED;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.MIRROR_ENABLED = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.MIRROR_ENABLED = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.MIRROR_ENABLED: ");
        USBSerial.println(CONFIG.MIRROR_ENABLED);
        tx_end();
      }
    }

    // Set Sweet Spot LOW threshold -------------------
    else if (strcmp(command_type, "sweet_spot_min") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MIN_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MIN_LEVEL;
      } else {
        CONFIG.SWEET_SPOT_MIN_LEVEL = constrain(atof(command_data), 0, uint32_t(-1));
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.SWEET_SPOT_MIN_LEVEL: ");
      USBSerial.println(CONFIG.SWEET_SPOT_MIN_LEVEL);
      tx_end();
    }

    // Set Sweet Spot HIGH threshold ------------------
    else if (strcmp(command_type, "sweet_spot_max") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MAX_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MAX_LEVEL;
      } else {
        CONFIG.SWEET_SPOT_MAX_LEVEL = constrain(atof(command_data), 0, uint32_t(-1));
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.SWEET_SPOT_MAX_LEVEL: ");
      USBSerial.println(CONFIG.SWEET_SPOT_MAX_LEVEL);
      tx_end();
    }

    // Set photons knob value -------------------
		else if (strcmp(command_type, "photons") == 0) {
      float value;
			if (strcmp(command_data, "default") == 0) {
				value = CONFIG_DEFAULTS.PHOTONS;
			} else {
				value = constrain(atof(command_data), 0.01, 1.0);
			}

			update_photons_chroma_mood(t_now, value, -1.0, -1.0);
			save_config_delayed();
			tx_begin();
			USBSerial.print("CONFIG.PHOTONS: ");
			USBSerial.println(CONFIG.PHOTONS);
			tx_end();
		}

    // Set mood knob value -------------------
		else if (strcmp(command_type, "mood") == 0) {
      float value;
			if (strcmp(command_data, "default") == 0) {
				value = CONFIG_DEFAULTS.MOOD;
			} else {
				value = constrain(atof(command_data), 0.01, 1.0);
			}

			update_photons_chroma_mood(t_now, -1.0, -1.0, value);
			save_config_delayed();
			tx_begin();
			USBSerial.print("CONFIG.MOOD: ");
			USBSerial.println(CONFIG.MOOD);
			tx_end();
		}

    // Set Chromagram Range ---------------
    else if (strcmp(command_type, "chromagram_range") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
      } else {
        CONFIG.CHROMAGRAM_RANGE = constrain(atof(command_data), 1, 64);
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.CHROMAGRAM_RANGE: ");
      USBSerial.println(CONFIG.CHROMAGRAM_RANGE);
      tx_end();
    }

    // Set Standby Dimming behavior -------
    else if (strcmp(command_type, "standby_dimming") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.STANDBY_DIMMING = CONFIG_DEFAULTS.STANDBY_DIMMING;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.STANDBY_DIMMING = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.STANDBY_DIMMING = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.STANDBY_DIMMING: ");
        USBSerial.println(CONFIG.STANDBY_DIMMING);
        tx_end();
      }
    }

    // Toggle bass mode -------------------
    else if (strcmp(command_type, "bass_mode") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        CONFIG.NOTE_OFFSET = 0;
        CONFIG.CHROMAGRAM_RANGE = 24;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.println("BASS MODE ENABLED");
        tx_end();
        reboot();
      }
    }

    // Set if image should be reversed ------------------------
    else if (strcmp(command_type, "reverse_order") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.REVERSE_ORDER = CONFIG_DEFAULTS.REVERSE_ORDER;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.REVERSE_ORDER = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.REVERSE_ORDER = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.REVERSE_ORDER: ");
        USBSerial.println(CONFIG.REVERSE_ORDER);
        tx_end();
      }
    }

    // Set max LED current ----------------------------
    else if (strcmp(command_type, "max_current_ma") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MAX_CURRENT_MA = CONFIG_DEFAULTS.MAX_CURRENT_MA;
      } else {
        CONFIG.MAX_CURRENT_MA = constrain(atof(command_data), 0, uint32_t(-1));
      }

      FastLED.setMaxPowerInVoltsAndMilliamps(5.0, CONFIG.MAX_CURRENT_MA);

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.MAX_CURRENT_MA: ");
      USBSerial.println(CONFIG.MAX_CURRENT_MA);
      tx_end();
    }

    // Stream a given value over Serial -----------------
    else if (strcmp(command_type, "stream") == 0) {
      stop_streams();  // Stop any current streams
      if (strcmp(command_data, "audio") == 0) {
        stream_audio = true;
        ack();
      } else if (strcmp(command_data, "fps") == 0) {
        stream_fps = true;
        ack();
      } else if (strcmp(command_data, "max_mags") == 0) {
        stream_max_mags = true;
        ack();
      } else if (strcmp(command_data, "max_mags_followers") == 0) {
        stream_max_mags_followers = true;
        ack();
      } else if (strcmp(command_data, "magnitudes") == 0) {
        stream_magnitudes = true;
        ack();
      } else if (strcmp(command_data, "spectrogram") == 0) {
        stream_spectrogram = true;
        ack();
      } else if (strcmp(command_data, "chromagram") == 0) {
        stream_chromagram = true;
        ack();
      } else {
        bad_command(command_type, command_data);
      }
    }

    // Toggle Color Shift ---------------------------------
    else if (strcmp(command_type, "auto_color_shift") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = CONFIG_DEFAULTS.AUTO_COLOR_SHIFT;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.AUTO_COLOR_SHIFT: ");
        USBSerial.println(CONFIG.AUTO_COLOR_SHIFT);
        tx_end();
      }
    }

    // Set Incandescent LUT intensity ----------------------------
    else if (strcmp(command_type, "incandescent_filter") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.INCANDESCENT_FILTER = CONFIG_DEFAULTS.INCANDESCENT_FILTER;
      } else {
        CONFIG.INCANDESCENT_FILTER = atof(command_data);
        if (CONFIG.INCANDESCENT_FILTER < 0.0) {
          CONFIG.INCANDESCENT_FILTER = 0.0;
        } else if (CONFIG.INCANDESCENT_FILTER > 1.0) {
          CONFIG.INCANDESCENT_FILTER = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.INCANDESCENT_FILTER: ");
      USBSerial.println(CONFIG.INCANDESCENT_FILTER);
      tx_end();
    }

    // Toggle Incandescent Mode ----------------------------
    else if (strcmp(command_type, "incandescent_mode") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.INCANDESCENT_MODE = CONFIG_DEFAULTS.INCANDESCENT_MODE;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.INCANDESCENT_MODE = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.INCANDESCENT_MODE = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        USBSerial.print("CONFIG.INCANDESCENT_MODE: ");
        USBSerial.println(CONFIG.INCANDESCENT_MODE);
        tx_end();
      }
    }

    // Set Bulb Cover Opacity ----------------------------
    else if (strcmp(command_type, "bulb_opacity") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BULB_OPACITY = CONFIG_DEFAULTS.BULB_OPACITY;
      } else {
        CONFIG.BULB_OPACITY = atof(command_data);
        if (CONFIG.BULB_OPACITY < 0.0) {
          CONFIG.BULB_OPACITY = 0.0;
        } else if (CONFIG.BULB_OPACITY > 1.0) {
          CONFIG.BULB_OPACITY = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.BULB_OPACITY: ");
      USBSerial.println(CONFIG.BULB_OPACITY);
      tx_end();
    }

    // Set Saturation ----------------------------
    else if (strcmp(command_type, "saturation") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SATURATION = CONFIG_DEFAULTS.SATURATION;
      } else {
        CONFIG.SATURATION = atof(command_data);
        if (CONFIG.SATURATION < 0.0) {
          CONFIG.SATURATION = 0.0;
        } else if (CONFIG.SATURATION > 1.0) {
          CONFIG.SATURATION = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      USBSerial.print("CONFIG.SATURATION: ");
      USBSerial.println(CONFIG.SATURATION);
      tx_end();
    }

    // Set Prism Count ----------------------------------------
    else if (strcmp(command_type, "prism_count") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        good = true;
        CONFIG.PRISM_COUNT = CONFIG_DEFAULTS.PRISM_COUNT;
      } else {
        good = true;
        CONFIG.PRISM_COUNT = constrain(atol(command_data), 0, 10);
      }

      if (good) {
        save_config();
        tx_begin();
        USBSerial.print("CONFIG.PRISM_COUNT: ");
        USBSerial.println(CONFIG.PRISM_COUNT);
        tx_end();
      }
    }

    // Set CONFIG preset ----------------------------
    else if (strcmp(command_type, "preset") == 0) {
      bool good = false;

      if      (strcmp(command_data, "default")      == 0) { good = true; }
      else if (strcmp(command_data, "tinted_bulbs") == 0) { good = true; }
      else if (strcmp(command_data, "incandescent") == 0) { good = true; }
      else if (strcmp(command_data, "white")        == 0) { good = true; }
      else if (strcmp(command_data, "classic")      == 0) { good = true; }

      else { // Bad preset name
        bad_command(command_type, command_data);
      }

      if (good) {
        set_preset(command_data); // presets.h

        save_config_delayed();
        tx_begin();
        USBSerial.print("ENABLED PRESET: ");
        USBSerial.println(command_data);
        tx_end();
      }
    }

    // COMMAND NOT RECOGNISED #############################
    else {
      bad_command(command_type, command_data);
    }
    // COMMAND NOT RECOGNISED #############################
  }
}

// Called on every frame, collects incoming characters until
// potential commands are found
void check_serial(uint32_t t_now) {
  serial_iter++;
  if (USBSerial.available() > 0) {
    char c = USBSerial.read();
		if ((c != '\r') && (c != '\n')) { // If normal character, add to buffer
      command_buf[command_buf_index] = c;
      command_buf_index++;

      // Avoid overflow at the cost of breaking a long command
      if (command_buf_index > 127) {
        command_buf_index = 127;
      }

    } else {  // a newline or linefeed character received,
			if (command_buf_index) {  // if we have any characters ...
        // the command in the buffer should be parsed
        parse_command(command_buf);                   // Parse
        memset(&command_buf, 0, sizeof(char) * 128);  // Clear
        command_buf_index = 0;                        // Reset
      }
    }
  }
}
