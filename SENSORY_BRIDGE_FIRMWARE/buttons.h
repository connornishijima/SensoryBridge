/*----------------------------------------
  Sensory Bridge BUTTON FUNCTIONS
----------------------------------------*/

// Every frame, we check both buttons for two types of input:
//
// NOISE BUTTON:
//    SHORT PRESS: BEGIN CALIBRATION
//    LONG PRESS:  CLEAR CALIBRATION
//
// MODE BUTTON:
//    SHORT PRESS: INCREMENT LIGHTSHOW MODE
//    LONG PRESS:  TOGGLE MIRRORING
//
// If buttons are touched on a unit that isn't set to MAIN,
// it will flash all units in the network (p2p.h) to identify
// which unit's buttons to touch instead to affect changes.

void check_buttons(uint32_t t_now) {
  if (digitalRead(noise_button.pin) == LOW) { // currently pressed
    if (noise_button.pressed == false) { // if just started
      noise_button.pressed = true;
      noise_button.last_down = t_now; // mark button-down time
    }

    if (t_now - noise_button.last_down > 250 && noise_button.last_up < noise_button.last_down) { // Still held, and for more than 250ms (long press)
      if (CONFIG.IS_MAIN_UNIT || main_override) { // If main, clear noise cal
        propagate_noise_reset();
        clear_noise_cal();
      } else { // if not, complain
        identify_main_unit();
      }

      noise_button.last_up = t_now; // count this event as a release time to prevent this function from repeating
    }
  } else if (digitalRead(noise_button.pin) == HIGH) { // button is not currently pressed
    if (noise_button.pressed == true) { // if it was just barely released
      noise_button.pressed = false;
      noise_button.last_up = t_now;

      uint32_t press_duration = noise_button.last_up - noise_button.last_down; // Get press duration

      if (press_duration < 250) { // if it was a short press
        if (CONFIG.IS_MAIN_UNIT || main_override) { // if main unit, start noise cal
          Serial.println("COLLECTING AMBIENT NOISE SAMPLES IN 500 MS");
          noise_complete = false;

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
        }
        else { // Otherwise, complain
          identify_main_unit();
        }
      }
    }
  }

  // Same as above, different button!
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
