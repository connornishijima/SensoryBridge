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

      if (press_duration <= 250) { // if it was a short press
        if (CONFIG.IS_MAIN_UNIT || main_override) { // if main unit, start noise cal
          noise_transition_queued = true; // See run_transition_fade() in led_utilities.h
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
        save_config_delayed();
      } else {
        identify_main_unit();
      }
      mode_button.last_up = t_now;
    }
  } else if (digitalRead(mode_button.pin) == HIGH) {
    if (mode_button.pressed == true) {
      mode_button.pressed = false;
      mode_button.last_up = t_now;
      bool skip_click = false;

      if (mode_transition_queued == true) {
        skip_click = true;
        mode_transition_queued = false;
        MASTER_BRIGHTNESS = 1.0;
        if(debug_mode){USBSerial.println("DOUBLE CLICK");}
#ifdef LUMOSSTICK
        // the LUMOSSTICK will use the double-click to toggle column first vs row first rendering
        if (CONFIG.IS_MAIN_UNIT || main_override) {
          CONFIG.LUMOS_ORDER = !CONFIG.LUMOS_ORDER;
          save_config_delayed();
        }
#endif
      }

      uint32_t press_duration = mode_button.last_up - mode_button.last_down;

      if (press_duration <= 250 && skip_click == false) {
        if (mode_transition_queued == false) {
          if (CONFIG.IS_MAIN_UNIT || main_override) {
            mode_transition_queued = true; // See run_transition_fade() in led_utilities.h
            mode_destination = -1;

            save_config_delayed();
          } else {
            identify_main_unit();
          }
        }
      }
    }
  }
}
