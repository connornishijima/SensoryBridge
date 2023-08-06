/*----------------------------------------
  Sensory Bridge FILESYSTEM ACCESS
  ----------------------------------------*/

extern void reboot(); // system.h

static int16_t _suspend_counter = 0;  // the counter allows for nested calls to 'suspend_leds()'

void suspend_led_thread(void) {
	_suspend_counter++;
	led_thread_halt = true;
  if (debug_mode) {
  	USBSerial.printf(" ... SUSPENDING LEDS %d ...\n", _suspend_counter);
  }
	delay(50); // allow led_thread() to complete its current cycle; cheap way to avoid the bridge problem
}

void resume_led_thread(void) {
	_suspend_counter--;
	if (_suspend_counter == 0)
		led_thread_halt = false; // allow LED thread to function
  if (debug_mode) {
  	USBSerial.printf(" ... RESUMING LEDS %d ... \n", _suspend_counter);
  }
}

void update_config_filename(uint32_t input) {
  snprintf(config_filename, 24, "/CONFIG_%05lu.BIN", input);
}

// Restore all defaults defined in globals.h by removing saved data and rebooting
void factory_reset() {
	USBSerial.print("Factory Reset: file system ");

  suspend_led_thread();
  bool success = LittleFS.format();
  resume_led_thread();

	if (success) {
		USBSerial.println("formatted");
	} else {
		USBSerial.println("format failed");
	}

  reboot();
}

// Restore only configuration defaults
void restore_defaults() {
#if 1
  factory_reset();    // this gives an easy way to start over
#else
  USBSerial.print("Deleting ");
  USBSerial.print(config_filename);
  USBSerial.print(": ");

  suspend_led_thread();
  if (LittleFS.remove(config_filename)) {
    USBSerial.println("file deleted");
  } else {
    USBSerial.println("delete failed");
  }
  resume_led_thread();

  reboot();
#endif
}

// Save configuration to LittleFS
void save_config() {
  if (debug_mode) {
    USBSerial.print("LITTLEFS: ");
  }
  
  suspend_led_thread();
  File file = LittleFS.open(config_filename, FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      USBSerial.print("Failed to open ");
      USBSerial.print(config_filename);
      USBSerial.println(" for writing!");
    }
  }
  if (file) {
    file.seek(0);
    uint8_t config_buffer[512];
    memcpy(config_buffer, &CONFIG, sizeof(CONFIG));

    for (uint16_t i = 0; i < 512; i++) {
      file.write(config_buffer[i]);
    }

    if (debug_mode) {
      USBSerial.print("WROTE ");
      USBSerial.print(config_filename);
      USBSerial.println(" SUCCESSFULLY");
    }
    file.close();
  }
  resume_led_thread();
}

// Save configuration to LittleFS 10 seconds from now
void save_config_delayed() {
  if(debug_mode == true){
    USBSerial.println("CONFIG SAVE QUEUED");
  }
  next_save_time = millis() + 15000;  // 15 seconds allows the user time to visualize their change and potentially change it again
  settings_updated = true;
}

// Load configuration from LittleFS
void load_config() {
  if (debug_mode) {
    USBSerial.print("LITTLEFS: ");
  }

  bool queue_factory_reset = false;

  suspend_led_thread();
  File file = LittleFS.open(config_filename, FILE_READ);
  if (!file) {
    if (debug_mode) {
      USBSerial.print("Failed to open ");
      USBSerial.print(config_filename);
      USBSerial.println(" for reading!");
    }
  }
  if (file) {
    file.seek(0);
    uint8_t config_buffer[512];
    for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
      config_buffer[i] = file.read();
    }

    memcpy(&CONFIG, config_buffer, sizeof(CONFIG));

    if (debug_mode) {
      USBSerial.println("READ CONFIG SUCCESSFULLY");
    }
    file.close();
  }
  resume_led_thread();

  if (queue_factory_reset == true) {
    factory_reset();
  }
}

// Save noise calibration to LittleFS
void save_ambient_noise_calibration() {
  if (debug_mode) {
    USBSerial.print("SAVING AMBIENT_NOISE PROFILE... ");
  }

  suspend_led_thread();
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      USBSerial.println("Failed to open file for writing!");
    }
  }
  if (file) {
    bytes_32 temp;

    file.seek(0);
    for (uint16_t i = 0; i < NUM_FREQS; i++) {
      float in_val = float(noise_samples[i]);

      temp.long_val_float = in_val;

      file.write(temp.bytes[0]);
      file.write(temp.bytes[1]);
      file.write(temp.bytes[2]);
      file.write(temp.bytes[3]);
    }

    file.close();
  }
  resume_led_thread();

  if (debug_mode) {
    USBSerial.println("SAVE COMPLETE");
  }
}

// Load noise calibration from LittleFS
void load_ambient_noise_calibration() {
  if (debug_mode) {
    USBSerial.print("LOADING AMBIENT_NOISE PROFILE... ");
  }

  suspend_led_thread();
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      USBSerial.println("Failed to open file for reading!");
    }
  }
  if (file) {
    bytes_32 temp;

    file.seek(0);
    for (uint16_t i = 0; i < NUM_FREQS; i++) {
      temp.bytes[0] = file.read();
      temp.bytes[1] = file.read();
      temp.bytes[2] = file.read();
      temp.bytes[3] = file.read();

      noise_samples[i] = SQ15x16(temp.long_val_float);
    }

    file.close();
  }
  resume_led_thread();

  if (debug_mode) {
    USBSerial.println("LOAD COMPLETE");
  }
}

// Initialize LittleFS
void init_fs() {
  suspend_led_thread();
  USBSerial.print("INIT FILESYSTEM: ");
  USBSerial.println(LittleFS.begin(true) == true ? PASS : FAIL);
  resume_led_thread();

  update_config_filename(FIRMWARE_VERSION);

  load_ambient_noise_calibration();
  load_config();
}
