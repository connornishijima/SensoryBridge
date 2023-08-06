/*----------------------------------------
  Sensory Bridge KNOB FUNCTIONS
----------------------------------------*/

void update_photons_chroma_mood(uint32_t t_now, float photons_value, float chroma_value, float mood_value) {
  // update the PHOTONS, CHROMA, and MOOD using either knob data or serial data from the user

  // If a value changes more than 5% and this unit is
  // not set to the MAIN unit, it will flash all units in the
  // network (p2p.h) to identify which unit's knobs to modify
  // instead to affect changes.

  static float PHOTONS_TARGET = 1.0;
  static float CHROMA_TARGET = 1.0;
  static float MOOD_TARGET = 1.0;

  static float PHOTONS_OUTPUT = 1.0;
  static float CHROMA_OUTPUT = 1.0;
  static float MOOD_OUTPUT = 1.0;

  static float PHOTONS_TARGET_LAST = 1.0;
  static float CHROMA_TARGET_LAST = 1.0;
  static float MOOD_TARGET_LAST = 1.0;

  if (!(photons_value < 0.0)) // a negative value means there is no new value for property
    PHOTONS_OUTPUT = photons_value;
  if (!(chroma_value < 0.0)) // a negative value means there is no new value for property
    CHROMA_OUTPUT = chroma_value;
  if (!(mood_value < 0.0)) // a negative value means there is no new value for property
    MOOD_OUTPUT = mood_value;

  // Happens every frame:
  if (PHOTONS_TARGET > PHOTONS_OUTPUT) {
    float delta = fabs(PHOTONS_TARGET - PHOTONS_OUTPUT);
    PHOTONS_OUTPUT += (delta * 0.1);
  } else if (PHOTONS_TARGET < PHOTONS_OUTPUT) {
    float delta = fabs(PHOTONS_TARGET - PHOTONS_OUTPUT);
    PHOTONS_OUTPUT -= (delta * 0.1);
  }

  if (CHROMA_TARGET > CHROMA_OUTPUT) {
    float delta = fabs(CHROMA_TARGET - CHROMA_OUTPUT);
    CHROMA_OUTPUT += (delta * 0.1);
  } else if (CHROMA_TARGET < CHROMA_OUTPUT) {
    float delta = fabs(CHROMA_TARGET - CHROMA_OUTPUT);
    CHROMA_OUTPUT -= (delta * 0.1);
  }

  if (MOOD_TARGET > MOOD_OUTPUT) {
    float delta = fabs(MOOD_TARGET - MOOD_OUTPUT);
    MOOD_OUTPUT += (delta * 0.1);
  } else if (MOOD_TARGET < MOOD_OUTPUT) {
    float delta = fabs(MOOD_TARGET - MOOD_OUTPUT);
    MOOD_OUTPUT -= (delta * 0.1);
  }

  if (CONFIG.IS_MAIN_UNIT || main_override) {  // If we're MAIN unit, show changed values
    CONFIG.PHOTONS = PHOTONS_OUTPUT;
    CONFIG.CHROMA = CHROMA_OUTPUT;
    CONFIG.MOOD = MOOD_OUTPUT;
  } else {  // If NOT MAIN, ignored changed values, flash all units to identify MAIN unit
    if (fabs(PHOTONS_TARGET - PHOTONS_TARGET_LAST) >= 0.05) {
      PHOTONS_TARGET_LAST = PHOTONS_TARGET;
      if (CONFIG.IS_MAIN_UNIT == false && main_override == false) {
        identify_main_unit();
      }
    }
    if (fabs(CHROMA_TARGET - CHROMA_TARGET_LAST) >= 0.05) {
      CHROMA_TARGET_LAST = CHROMA_TARGET;
      if (CONFIG.IS_MAIN_UNIT == false && main_override == false) {
        identify_main_unit();
      }
    }
    if (fabs(MOOD_TARGET - MOOD_TARGET_LAST) >= 0.05) {
      MOOD_TARGET_LAST = MOOD_TARGET;
      if (CONFIG.IS_MAIN_UNIT == false && main_override == false) {
        identify_main_unit();
      }
    }
  }

  // CHROMA Knob handling
  chroma_val = 1.0;
  if (CONFIG.CHROMA < 0.95) {
    chroma_val = CONFIG.CHROMA * 1.05263157;  // Reciprocal of 0.95 above
    chromatic_mode = false;
  } else {
    chromatic_mode = true;
  }

  // Mood knob processing
  float smoothing_top_half = (CONFIG.MOOD - 0.5);
  if (smoothing_top_half < 0.0) {
    smoothing_top_half = 0.0;
  }
  smoothing_top_half *= 2.0;
  // Top half of knob now has range of 0.0 (knob is centered) to 1.0 (knob fully right) accesible through this variable (stays 0.0 when in bottom half)

  float smoothing_bottom_half = (CONFIG.MOOD - 0.5);
  smoothing_bottom_half *= 2.0;
  if (smoothing_bottom_half > 0.0) {
    smoothing_bottom_half = 0.0;
  }
  smoothing_bottom_half *= -1.0;
  smoothing_bottom_half = 1.0 - smoothing_bottom_half;
  smoothing_bottom_half = (smoothing_bottom_half * 0.9) + 0.1;  // 0.0-1.0 input range becomes 0.1-1.0
  // Bottom half of knob now has range of 0.1 (fully left) to 1.0 (knob is centered) accesible through this variable (stays 1.0 when in top half)
  // Making 0.1 the bottom value prevents the LEDs from experiencing 0% change per frame! ;)

  // These are the final values we'll feed into the two smoothing algorithms soon
  smoothing_follower = 0.100 + (smoothing_top_half * 0.300);  // 0.0-1.0 input range becomes 0.1 - 0.400
  smoothing_exp_average = 1.0 - smoothing_bottom_half;        // invert input

  // Process knob speed and update knob structs
  knob_photons.value = CONFIG.PHOTONS;
  knob_chroma.value = CONFIG.CHROMA;
  knob_mood.value = CONFIG.MOOD;

  SQ15x16 speed_threshold = 0.005;

  knob_photons.change_rate = fabs_fixed(knob_photons.value - knob_photons.last_value);
  knob_chroma.change_rate = fabs_fixed(knob_chroma.value - knob_chroma.last_value);
  knob_mood.change_rate = fabs_fixed(knob_mood.value - knob_mood.last_value);

  if (knob_photons.change_rate > speed_threshold) { knob_photons.last_change = t_now; }
  if (knob_chroma.change_rate > speed_threshold) { knob_chroma.last_change = t_now; }
  if (knob_mood.change_rate > speed_threshold) { knob_mood.last_change = t_now; }

  uint16_t knob_timeout_ms = 1500;

  uint32_t most_recent_time = 0;
  uint8_t most_recent_knob = K_NONE;
  if ((t_now - knob_photons.last_change) <= knob_timeout_ms && knob_photons.last_change > most_recent_time) {
    most_recent_time = knob_photons.last_change;
    most_recent_knob = K_PHOTONS;
  }
  if ((t_now - knob_chroma.last_change)  <= knob_timeout_ms && knob_chroma.last_change  > most_recent_time) {
    most_recent_time = knob_chroma.last_change;
    most_recent_knob = K_CHROMA;
  }
  if ((t_now - knob_mood.last_change)    <= knob_timeout_ms && knob_mood.last_change    > most_recent_time) {
    most_recent_time = knob_mood.last_change;
    most_recent_knob = K_MOOD;
  }

  current_knob = most_recent_knob;

  knob_photons.last_value = knob_photons.value;
  knob_chroma.last_value = knob_chroma.value;
  knob_mood.last_value = knob_mood.value;
}


uint16_t avg_read(uint8_t pin) {
  uint32_t sum = 0;
  sum += analogRead(pin);
  sum += analogRead(pin);
  return sum >> 1;
}

void check_knobs(uint32_t t_now) {
  // On every other frame, the knobs are read. The resulting
  // values are put through the same "follower" smoothing
  // algorithm detailed in GDFT.h.
  static uint32_t iter = 0;

  iter++;
  float photons_value = -1.0, chroma_value = -1.0, mood_value = -1.0;

  if (iter % 1 == 0) {  // If frame count is multiple of 2
#ifdef PHOTONS_PIN
    photons_value = (1.0 - (avg_read(PHOTONS_PIN) / 8192.0));
#endif
#ifdef CHROMA_PIN
    chroma_value = (1.0 - (avg_read(CHROMA_PIN) / 8192.0));
#endif
#ifdef MOOD_PIN
    mood_value = (1.0 - (avg_read(MOOD_PIN) / 8192.0));
#endif
  }

  update_photons_chroma_mood(t_now, photons_value, chroma_value, mood_value);
}
