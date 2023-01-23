/*----------------------------------------
  Sensory Bridge UART COMMAND LINE
  ----------------------------------------*/

// I want this to be more fully featured in the future, but for now:
//
// These functions watch the Serial port for incoming commands,
// and perform actions based on whatis recieved.

extern void check_current_function();

void tx_begin(bool error = false){
  if(error == false){
    Serial.println("sbr{{");
  }
  else{
    Serial.println("sberr[[");
  }
}

void tx_end(bool error = false){
  if(error == false){
    Serial.println("}}");
  }
  else{
    Serial.println("]]");
  }
}

void ack(){
  Serial.println("SBOK");
}

void bad_command(char* command_type, char* command_data) {
  tx_begin(true);
  Serial.print("Bad command: ");
  Serial.print(command_type);
  if (command_data[0] != 0) {
    Serial.print("=");
    Serial.print(command_data);
  }

  Serial.println();
  tx_end(true);
}

void stop_streams() {
  stream_audio = false;
  stream_fps = false;
  stream_max_mags = false;
  stream_max_mags_followers = false;
  stream_magnitudes = false;
  stream_spectrogram = false;
}

void init_serial(uint32_t baud_rate) {
  Serial.begin(baud_rate);  // Default 500,000 baud
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
  Serial.println("---------------------------");
  Serial.print("SENSORY BRIDGE | VER: ");
  Serial.println(FIRMWARE_VERSION);
  Serial.println("---------------------------");
  Serial.println();
  Serial.print("INIT_SERIAL: ");
  Serial.println(serial_started == true ? PASS : FAIL);
}

// This is for development purposes, and allows the user to dump
// the current values of many variables to the monitor at once
void dump_info() {
  Serial.print("FIRMWARE_VERSION: ");
  Serial.println(FIRMWARE_VERSION);

  Serial.print("CHIP ID: ");
  Serial.println(chip_id);

  Serial.print("noise_button.pressed: ");
  Serial.println(noise_button.pressed);

  Serial.print("noise_button.last_down: ");
  Serial.println(noise_button.last_down);

  Serial.print("noise_button.last_up: ");
  Serial.println(noise_button.last_up);

  Serial.print("noise_button.pin: ");
  Serial.println(noise_button.pin);

  Serial.print("mode_button.pressed: ");
  Serial.println(mode_button.pressed);

  Serial.print("mode_button.last_down: ");
  Serial.println(mode_button.last_down);

  Serial.print("mode_button.last_up: ");
  Serial.println(mode_button.last_up);

  Serial.print("mode_button.pin: ");
  Serial.println(mode_button.pin);

  Serial.print("CONFIG.PHOTONS: ");
  Serial.println(CONFIG.PHOTONS, 6);

  Serial.print("CONFIG.CHROMA: ");
  Serial.println(CONFIG.CHROMA, 6);

  Serial.print("CONFIG.MOOD: ");
  Serial.println(CONFIG.MOOD, 6);

  Serial.print("CONFIG.LIGHTSHOW_MODE: ");
  Serial.println(CONFIG.LIGHTSHOW_MODE);

  Serial.print("CONFIG.MIRROR_ENABLED: ");
  Serial.println(CONFIG.MIRROR_ENABLED);

  Serial.print("CONFIG.CHROMAGRAM_RANGE: ");
  Serial.println(CONFIG.CHROMAGRAM_RANGE);

  Serial.print("CONFIG.SAMPLE_RATE: ");
  Serial.println(CONFIG.SAMPLE_RATE);

  Serial.print("CONFIG.NOTE_OFFSET: ");
  Serial.println(CONFIG.NOTE_OFFSET);

  Serial.print("CONFIG.SQUARE_ITER: ");
  Serial.println(CONFIG.SQUARE_ITER);

  Serial.print("CONFIG.MAGNITUDE_FLOOR: ");
  Serial.println(CONFIG.MAGNITUDE_FLOOR);

  Serial.print("CONFIG.LED_TYPE: ");
  Serial.println(CONFIG.LED_TYPE);

  Serial.print("CONFIG.LED_COUNT: ");
  Serial.println(CONFIG.LED_COUNT);

  Serial.print("CONFIG.LED_COLOR_ORDER: ");
  Serial.println(CONFIG.LED_COLOR_ORDER);

  Serial.print("CONFIG.MAX_BLOCK_SIZE: ");
  Serial.println(CONFIG.MAX_BLOCK_SIZE);

  Serial.print("CONFIG.SAMPLES_PER_CHUNK: ");
  Serial.println(CONFIG.SAMPLES_PER_CHUNK);

  Serial.print("CONFIG.GAIN: ");
  Serial.println(CONFIG.GAIN, 6);

  Serial.print("CONFIG.BOOT_ANIMATION: ");
  Serial.println(CONFIG.BOOT_ANIMATION);

  Serial.print("CONFIG.SWEET_SPOT_MIN_LEVEL: ");
  Serial.println(CONFIG.SWEET_SPOT_MIN_LEVEL);

  Serial.print("CONFIG.SWEET_SPOT_MAX_LEVEL: ");
  Serial.println(CONFIG.SWEET_SPOT_MAX_LEVEL);

  Serial.print("CONFIG.DC_OFFSET: ");
  Serial.println(CONFIG.DC_OFFSET);

<<<<<<< Updated upstream
  Serial.print("CONFIG.ESPNOW_CHANNEL: ");
  Serial.println(CONFIG.ESPNOW_CHANNEL);
=======
  Serial.print("CONFIG.STANDBY_DIMMING: ");
  Serial.println(CONFIG.STANDBY_DIMMING);
>>>>>>> Stashed changes

  Serial.print("CONFIG.IS_MAIN_UNIT: ");
  Serial.println(CONFIG.IS_MAIN_UNIT);

  Serial.print("CONFIG.VERSION: ");
  Serial.println(CONFIG.VERSION);

  Serial.print("MASTER_BRIGHTNESS: ");
  Serial.println(MASTER_BRIGHTNESS);

  Serial.print("stream_audio: ");
  Serial.println(stream_audio);

  Serial.print("stream_fps: ");
  Serial.println(stream_fps);

  Serial.print("stream_max_mags: ");
  Serial.println(stream_max_mags);

  Serial.print("stream_max_mags_followers: ");
  Serial.println(stream_max_mags_followers);

  Serial.print("stream_magnitudes: ");
  Serial.println(stream_magnitudes);

  Serial.print("stream_spectrogram: ");
  Serial.println(stream_spectrogram);

  Serial.print("debug_mode: ");
  Serial.println(debug_mode);

  Serial.print("noise_complete: ");
  Serial.println(noise_complete);

  Serial.print("noise_iterations: ");
  Serial.println(noise_iterations);

  Serial.print("main_override: ");
  Serial.println(main_override);

  Serial.print("last_rx_time: ");
  Serial.println(last_rx_time);

  Serial.print("last_setting_change: ");
  Serial.println(last_setting_change);

  Serial.print("settings_updated: ");
  Serial.println(settings_updated);

  Serial.print("I2S_PORT: ");
  Serial.println(I2S_PORT);

  Serial.print("SAMPLE_HISTORY_LENGTH: ");
  Serial.println(SAMPLE_HISTORY_LENGTH);

  Serial.print("silence: ");
  Serial.println(silence);

  Serial.print("mode_destination: ");
  Serial.println(mode_destination);

  Serial.print("AVERAGE FPS: ");
  Serial.println(FPS);
}

// This parses a completed command to decide how to handle it
void parse_command(char* command_buf) {

  // COMMANDS WITHOUT METADATA ###############################

  // Get firmware version -----------------------------------
  if (strcmp(command_buf, "v") == 0 || strcmp(command_buf, "V") == 0 || strcmp(command_buf, "version") == 0) {

    tx_begin();
    Serial.print("VERSION: ");
    Serial.println(FIRMWARE_VERSION);
    tx_end();

  }

  // Print help ---------------------------------------------
  else if (strcmp(command_buf, "h") == 0 || strcmp(command_buf, "H") == 0 || strcmp(command_buf, "help") == 0) {

    tx_begin();
    Serial.println("SENSORY BRIDGE - Serial Menu ------------------------------------------------------------------------------------");
    Serial.println();
    Serial.println("                                           v | Print firmware version number");
    Serial.println("                                       reset | Reboot Sensory Bridge");
    Serial.println("                               factory_reset | Delete configuration, including noise cal, reboot");
    Serial.println("                            restore_defaults | Delete configuration, reboot");
    Serial.println("                               get_main_unit | Print if this unit is set to MAIN for SensorySync");
    Serial.println("                                        dump | Print tons of useful variables in realtime");
    Serial.println("                                        stop | Stops the output of any enabled streams");
    Serial.println("                                         fps | Return the average FPS");
    Serial.println("                                     chip_id | Return the chip id (MAC) of the CPU");
    Serial.println("                                    get_mode | Get lightshow mode's ID (index)");
    Serial.println("                               get_num_modes | Return the number of modes available");
    Serial.println("                             start_noise_cal | Remotely begin a noise calibration");
    Serial.println("                             clear_noise_cal | Remotely clear the stored noise calibration");
    Serial.println("                              set_mode=[int] | Set the mode number");
    Serial.println("         mirror_enabled=[true/false/default] | Remotely toggle lightshow mirroring");
    Serial.println("                         get_mode_name=[int] | Get a mode's name by ID (index)");
    Serial.println("                               stream=[type] | Stream live data to a Serial Plotter.");
    Serial.println("                                               Options are: audio, fps, max_mags, max_mags_followers, magnitudes, spectrogram");
    Serial.println("             led_type=['neopixel'/'dotstar'] | Sets which LED protocol to use, 3 wire or 4 wire");
    Serial.println("                led_count=[int or 'default'] | Sets how many LEDs your display will use (native resolution is 128)");
    Serial.println("       led_color_order=[GRB/RGB/BGR/default] | Sets LED color ordering, default GRB");
    Serial.println("      led_interpolation=[true/false/default] | Toggles linear LED interpolation when running in a non-native resolution (slower)");
    Serial.println("                          debug=[true/false] | Enables debug mode, where functions are timed");
    Serial.println("               sample_rate=[hz or 'default'] | Sets the microphone sample rate");
    Serial.println("             note_offset=[0-32 or 'default'] | Sets the lowest note, as a positive offset from A1 (55.0Hz)");
    Serial.println("              square_iter=[int or 'default'] | Sets the number of times the LED output is squared (contrast)");
    Serial.println("          magnitude_floor=[int or 'default'] | Sets minimum magnitude a frequency bin must have to contribute the show");
    Serial.println("           max_block_size=[int or 'default'] | Sets the maximum number of samples used to compute frequency data");
    Serial.println("        samples_per_chunk=[int or 'default'] | Sets the number of samples collected every frame");
    Serial.println("                   gain=[float or 'default'] | Sets the scaling of spectrogram data (>0.0 is brighter, <0.0 is darker)");
    Serial.println("         boot_animation=[true/false/default] | Enable or disable the boot animation");
    Serial.println("                  set_main_unit=[true/false] | Sets if this unit is MAIN or not for SensorySync");
    Serial.println("           sweet_spot_min=[int or 'default'] | Sets the minimum amplitude to be inside the 'Sweet Spot'");
    Serial.println("           sweet_spot_max=[int or 'default'] | Sets the maximum amplitude to be inside the 'Sweet Spot'");
    Serial.println("        chromagram_range=[1-64 or 'default'] | Range between 1 and 64, how many notes at the bottom of the");
    Serial.println("                                               spectrogram should be considered in chromagram sums");
    Serial.println("        standby_dimming=[true/false/default] | Toggle dimming during detected silence");
    Serial.println("                      bass_mode=[true/false] | Toggle bass-mode, which alters note_offset and chromagram_range for bass-y tunes");
    tx_end();
  }

  // So that software can automatically identify this device -
  else if (strcmp(command_buf, "SB?") == 0) {

    Serial.println("SB!");

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
    Serial.print("CHIP_ID: ");
    Serial.println(chip_id);
    tx_end();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "start_noise_cal") == 0) {

    ack();
    start_noise_cal();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "clear_noise_cal") == 0) {

    ack();
    clear_noise_cal();

  }

  // Returns the number of modes available ------------------
  else if (strcmp(command_buf, "get_num_modes") == 0) {

    tx_begin();
    Serial.print("NUM_MODES: ");
    Serial.println(NUM_MODES);
    tx_end();

  }

  // Returns the mode ID ------------------------------------
  else if (strcmp(command_buf, "get_mode") == 0) {

    tx_begin();
    Serial.print("MODE: ");
    Serial.println(CONFIG.LIGHTSHOW_MODE);
    tx_end();

  }

  // Returns whether or not this is a "MAIN" unit -----------
  else if (strcmp(command_buf, "get_main_unit") == 0) {

    tx_begin();
    Serial.print("CONFIG.IS_MAIN_UNIT: ");
    Serial.println(CONFIG.IS_MAIN_UNIT);
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
    Serial.print("FPS: ");
    Serial.println(FPS);
    tx_end();
    
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
      }
      else{
        bad_command(command_type, command_data);
      }

      if(good){
        save_config();
  
        tx_begin();
        Serial.print("CONFIG.IS_MAIN_UNIT: ");
        Serial.println(CONFIG.IS_MAIN_UNIT);
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

      if(good){
        tx_begin();
        Serial.print("debug_mode: ");
        Serial.println(debug_mode);
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
        CONFIG.SAMPLE_RATE = constrain(atol(command_data),8000,44100);
      }

      if(good){
        save_config();
        tx_begin();
        Serial.print("CONFIG.SAMPLE_RATE: ");
        Serial.println(CONFIG.SAMPLE_RATE);
        tx_end();
        reboot();
      }
    }

    // Set Mode Number ----------------------------------------
    else if (strcmp(command_type, "set_mode") == 0) {
      mode_transition_queued = true;
      mode_destination = constrain(atol(command_data),0,NUM_MODES-1);

      save_config_delayed();
      tx_begin();
      Serial.print("CONFIG.LIGHTSHOW_MODE: ");
      Serial.println(mode_destination);
      tx_end();
    }

    // Get Mode Name By ID ------------------------------------
    else if (strcmp(command_type, "get_mode_name") == 0) {
      uint16_t mode_id = atol(command_data);

      if(mode_id < NUM_MODES){
        char buf[32] = { 0 };
        for(uint8_t i = 0; i < 32; i++){
          char c = mode_names[32*mode_id + i];
          if(c != 0){
            buf[i] = c;
          }
          else{
            break;
          }
        }
  
        tx_begin();
        Serial.print("MODE_NAME: ");
        Serial.println(buf);
        tx_end();
      }
      else{
        bad_command(command_type, command_data);
      }
    }

    // Set Note Offset ----------------------------------------
    else if (strcmp(command_type, "note_offset") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
      } else {
        CONFIG.NOTE_OFFSET = constrain(atol(command_data),0,32);
      }
      save_config();
      tx_begin();
      Serial.print("CONFIG.NOTE_OFFSET: ");
      Serial.println(CONFIG.NOTE_OFFSET);
      tx_end();
      reboot();
    }

    // Set Square Iterations ----------------------------------
    else if (strcmp(command_type, "square_iter") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SQUARE_ITER = CONFIG_DEFAULTS.SQUARE_ITER;
      } else {
        CONFIG.SQUARE_ITER = constrain(atol(command_data),0,10);
      }
      save_config_delayed();
      
      tx_begin();
      Serial.print("CONFIG.SQUARE_ITER: ");
      Serial.println(CONFIG.SQUARE_ITER);
      tx_end();
    }

    // Set Magnitude Floor -----------------------------------
    else if (strcmp(command_type, "magnitude_floor") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MAGNITUDE_FLOOR = CONFIG_DEFAULTS.MAGNITUDE_FLOOR;
      } else {
        CONFIG.MAGNITUDE_FLOOR = constrain(atol(command_data),0,uint32_t(-1));
      }
      save_config_delayed();
      
      tx_begin();
      Serial.print("CONFIG.MAGNITUDE_FLOOR: ");
      Serial.println(CONFIG.MAGNITUDE_FLOOR);
      tx_end();
    }

    // Set LED Type ---------------------------------------
    else if (strcmp(command_type, "led_type") == 0) {
      bool good = false;
      if (strcmp(command_data, "neopixel") == 0) {
        CONFIG.LED_TYPE = LED_NEOPIXEL;
        good = true;
      } else if (strcmp(command_data, "dotstar") == 0) {
        CONFIG.LED_TYPE = LED_DOTSTAR;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config();
        tx_begin();
        Serial.print("CONFIG.LED_TYPE: ");
        Serial.println(CONFIG.LED_TYPE);
        tx_end();
        reboot();
      }
    }

    // Set LED Count ------------------------------------
    else if (strcmp(command_type, "led_count") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COUNT = CONFIG_DEFAULTS.LED_COUNT;
      } else {
        CONFIG.LED_COUNT = constrain(atol(command_data),1,10000);
      }
      
      save_config();
      tx_begin();
      Serial.print("CONFIG.LED_COUNT: ");
      Serial.println(CONFIG.LED_COUNT);
      tx_end();
      reboot();
    }

    // Set LED Interpolation ----------------------------
    else if (strcmp(command_type, "led_interpolation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_INTERPOLATION = CONFIG_DEFAULTS.LED_INTERPOLATION;
        good = true;
      }
      else if (strcmp(command_data, "true") == 0) {
        CONFIG.LED_INTERPOLATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.LED_INTERPOLATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config_delayed();
        tx_begin();
        Serial.print("CONFIG.LED_INTERPOLATION: ");
        Serial.println(CONFIG.LED_INTERPOLATION);
        tx_end();
      }
    }

    // Set LED Color Order ----------------------------
    else if (strcmp(command_type, "led_color_order") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COLOR_ORDER = CONFIG_DEFAULTS.LED_COLOR_ORDER;
        good = true;
      }
      else if (strcmp(command_data, "GRB") == 0) {
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

      if(good){
        save_config();
        tx_begin();
        Serial.print("CONFIG.LED_COLOR_ORDER: ");
        Serial.println(CONFIG.LED_COLOR_ORDER);
        tx_end();
        reboot();
      }
    }

    // Set Max Block Size ------------------------------
    else if (strcmp(command_type, "max_block_size") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MAX_BLOCK_SIZE = CONFIG_DEFAULTS.MAX_BLOCK_SIZE;
      } else {
        CONFIG.MAX_BLOCK_SIZE = constrain(atol(command_data),1,SAMPLE_HISTORY_LENGTH);
      }
      
      save_config();
      tx_begin();
      Serial.print("CONFIG.MAX_BLOCK_SIZE: ");
      Serial.println(CONFIG.MAX_BLOCK_SIZE);
      tx_end();
      reboot();
    }

    // Set Samples Per Chunk ---------------------------
    else if (strcmp(command_type, "samples_per_chunk") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SAMPLES_PER_CHUNK = CONFIG_DEFAULTS.SAMPLES_PER_CHUNK;
      } else {
        CONFIG.SAMPLES_PER_CHUNK = constrain(atol(command_data),0,SAMPLE_HISTORY_LENGTH);
      }
      
      save_config();
      tx_begin();
      Serial.print("CONFIG.SAMPLES_PER_CHUNK: ");
      Serial.println(CONFIG.SAMPLES_PER_CHUNK);
      tx_end();
      reboot();
    }

    // Set Spectrogram Gain ----------------------------
    else if (strcmp(command_type, "gain") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.GAIN = CONFIG_DEFAULTS.GAIN;
      } else {
        CONFIG.GAIN = atof(command_data);
      }
      
      save_config_delayed();
      tx_begin();
      Serial.print("CONFIG.GAIN: ");
      Serial.println(CONFIG.GAIN);
      tx_end();
    }

    // Toggle Boot Animation --------------------------
    else if (strcmp(command_type, "boot_animation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BOOT_ANIMATION = CONFIG_DEFAULTS.BOOT_ANIMATION;
        good = true;
      }
      else if (strcmp(command_data, "true") == 0) {
        CONFIG.BOOT_ANIMATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.BOOT_ANIMATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config();
        tx_begin();
        Serial.print("CONFIG.BOOT_ANIMATION: ");
        Serial.println(CONFIG.BOOT_ANIMATION);
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
      }
      else if (strcmp(command_data, "true") == 0) {
        CONFIG.MIRROR_ENABLED = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.MIRROR_ENABLED = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config_delayed();
        tx_begin();
        Serial.print("CONFIG.MIRROR_ENABLED: ");
        Serial.println(CONFIG.MIRROR_ENABLED);
        tx_end();
      }
    }

    // Set Sweet Spot LOW threshold -------------------
    else if (strcmp(command_type, "sweet_spot_min") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MIN_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MIN_LEVEL;
      }
      else {
        CONFIG.SWEET_SPOT_MIN_LEVEL = constrain(atof(command_data),0,uint32_t(-1));
      }
      
      save_config_delayed();
      tx_begin();
      Serial.print("CONFIG.SWEET_SPOT_MIN_LEVEL: ");
      Serial.println(CONFIG.SWEET_SPOT_MIN_LEVEL);
      tx_end();
    }

    // Set Sweet Spot HIGH threshold ------------------
    else if (strcmp(command_type, "sweet_spot_max") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MAX_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MAX_LEVEL;
      }
      else {
        CONFIG.SWEET_SPOT_MAX_LEVEL = constrain(atof(command_data),0,uint32_t(-1));
      }
      
      save_config_delayed();
      tx_begin();
      Serial.print("CONFIG.SWEET_SPOT_MAX_LEVEL: ");
      Serial.println(CONFIG.SWEET_SPOT_MAX_LEVEL);
      tx_end();
    }

    // Set Chromagram Range ---------------
    else if (strcmp(command_type, "chromagram_range") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
      }
      else {
        CONFIG.CHROMAGRAM_RANGE = constrain(atof(command_data),1,64);
      }
      
      save_config_delayed();
      tx_begin();
      Serial.print("CONFIG.CHROMAGRAM_RANGE: ");
      Serial.println(CONFIG.CHROMAGRAM_RANGE);
      tx_end();
    }

    // Set Standby Dimming behavior -------
    else if (strcmp(command_type, "standby_dimming") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.STANDBY_DIMMING = CONFIG_DEFAULTS.STANDBY_DIMMING;
        good = true;
      }
      else if (strcmp(command_data, "true") == 0) {
        CONFIG.STANDBY_DIMMING = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.STANDBY_DIMMING = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config_delayed();
        tx_begin();
        Serial.print("CONFIG.STANDBY_DIMMING: ");
        Serial.println(CONFIG.STANDBY_DIMMING);
        tx_end();
      }
    }

    // Toggle bass mode -------------------
    else if (strcmp(command_type, "bass_mode") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        CONFIG.NOTE_OFFSET = 0;
        CONFIG.CHROMAGRAM_RANGE = 18;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if(good){
        save_config();
        tx_begin();
        Serial.println("BASS MODE ENABLED");
        tx_end();
        reboot();
      }
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
      } else {
        bad_command(command_type, command_data);
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
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n') {  // If normal character, add to buffer
      command_buf[command_buf_index] = c;
      command_buf_index++;

      // Avoid overflow at the cost of breaking a long command
      if (command_buf_index > 127) {
        command_buf_index = 127;
      }

    } else {  // If a newline character is received,
      // the command in the buffer should be parsed
      parse_command(command_buf);                   // Parse
      memset(&command_buf, 0, sizeof(char) * 128);  // Clear
      command_buf_index = 0;                        // Reset
    }
  }
}
