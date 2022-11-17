/*----------------------------------------
  Sensory Bridge BUTTON FUNCTIONS
----------------------------------------*/

void check_buttons(uint32_t t_now) {
  if (digitalRead(noise_button.pin) == LOW) {
    if (noise_button.pressed == false) {
      noise_button.pressed = true;
      noise_button.last_down = t_now;
    }

    if (t_now - noise_button.last_down > 250 && noise_button.last_up < noise_button.last_down) {
      if (CONFIG.IS_MAIN_UNIT || main_override) {
        propagate_noise_reset();
        clear_noise_cal();
      } else {
        identify_main_unit();
      }

      noise_button.last_up = t_now;
    }
  } else if (digitalRead(noise_button.pin) == HIGH) {
    if (noise_button.pressed == true) {
      noise_button.pressed = false;
      noise_button.last_up = t_now;

      uint32_t press_duration = noise_button.last_up - noise_button.last_down;

      if (press_duration < 250) {
        if (CONFIG.IS_MAIN_UNIT || main_override) {
          Serial.println("COLLECTING AMBIENT NOISE SAMPLES IN 500 MS");
          collecting_ambient_noise = true;

          clear_all_led_buffers();
          CONFIG.DC_OFFSET = 0;
          CONFIG.WAVEFORM_NOISE_FLOOR = 0;

          for (uint16_t i = 0; i < 128; i++) {
            for (uint16_t x = 0; x < 128; x++) {
              leds[x] = CRGB(0, 0, 0);
            }
            uint16_t ir = 127 - i;
            leds[ir] = CRGB(0, 255, 255);
            show_leds();
          }

          clear_all_led_buffers();
          show_leds();

          propagate_noise_cal();
          start_noise_cal();
        } else {
          identify_main_unit();
        }
      }
    }
  }

  if (digitalRead(mode_button.pin) == LOW) {
    if (mode_button.pressed == false) {
      mode_button.pressed = true;
      mode_button.last_down = t_now;
    }

    if (t_now - mode_button.last_down > 250 && mode_button.last_up < mode_button.last_down) {
      if (CONFIG.IS_MAIN_UNIT || main_override) {
        CONFIG.MIRROR_ENABLED = !CONFIG.MIRROR_ENABLED;
        last_setting_change = millis();
        settings_updated = true;
      } else {
        identify_main_unit();
      }
      mode_button.last_up = t_now;
    }
  } else if (digitalRead(mode_button.pin) == HIGH) {
    if (mode_button.pressed == true) {
      mode_button.pressed = false;
      mode_button.last_up = t_now;

      uint32_t press_duration = mode_button.last_up - mode_button.last_down;

      if (press_duration < 250) {
        if (CONFIG.IS_MAIN_UNIT || main_override) {
          CONFIG.LIGHTSHOW_MODE++;
          if (CONFIG.LIGHTSHOW_MODE >= NUM_MODES) {
            CONFIG.LIGHTSHOW_MODE = 0;
          }

          last_setting_change = millis();
          settings_updated = true;
        } else {
          identify_main_unit();
        }
      }
    }
  }
}