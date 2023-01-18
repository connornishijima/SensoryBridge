/*----------------------------------------
  Sensory Bridge KNOB FUNCTIONS
----------------------------------------*/

void check_knobs(uint32_t t_now) {
  // On every 10th frame, the knobs are read. The resulting
  // values are put through the same "follower" smoothing
  // algorithm detailed in GDFT.h.
  //
  // If a knob value changes more than 5% and this unit is
  // not set to the MAIN unit, it will flash all units in the
  // network (p2p.h) to identify which unit's knobs to modify
  // instead to affect changes.
  
  static uint32_t iter = 0;
  static float PHOTONS_TARGET = 1.0;
  static float CHROMA_TARGET = 1.0;
  static float MOOD_TARGET = 1.0;

  static float PHOTONS_OUTPUT = 1.0;
  static float CHROMA_OUTPUT = 1.0;
  static float MOOD_OUTPUT = 1.0;

  static float PHOTONS_TARGET_LAST = 1.0;
  static float CHROMA_TARGET_LAST = 1.0;
  static float MOOD_TARGET_LAST = 1.0;

  iter++;

  if (iter % 10 == 0) { // If frame count is multiple of 10
    PHOTONS_TARGET = (1.0 - (analogRead(PHOTONS_PIN) / 8192.0));
    CHROMA_TARGET  = (1.0 - (analogRead(CHROMA_PIN)  / 8192.0));
    MOOD_TARGET    = (1.0 - (analogRead(MOOD_PIN)    / 8192.0));
  }

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

  if (CONFIG.IS_MAIN_UNIT || main_override) { // If we're MAIN unit, show changed values
    CONFIG.PHOTONS = PHOTONS_OUTPUT;
    CONFIG.CHROMA = CHROMA_OUTPUT;
    CONFIG.MOOD = MOOD_OUTPUT;
  }
  else { // If NOT MAIN, ignored changed values, flash all units to identify MAIN unit
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

  // This is only used to fade in when booting!
  if(t_now >= 1000){
    if(MASTER_BRIGHTNESS < 1.0){
      MASTER_BRIGHTNESS += 0.01;
    }
    if(MASTER_BRIGHTNESS > 1.0){
      MASTER_BRIGHTNESS = 1.00;
    }
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
  smoothing_bottom_half = (smoothing_bottom_half * 0.9) + 0.1; // 0.0-1.0 input range becomes 0.1-1.0
  // Bottom half of knob now has range of 0.1 (fully left) to 1.0 (knob is centered) accesible through this variable (stays 1.0 when in top half)
  // Making 0.1 the bottom value prevents the LEDs from experiencing 0% change per frame! ;)

  // These are the final values we'll feed into the two smoothing algorithms soon
  smoothing_follower    = 0.225 + (smoothing_top_half * 0.275); // 0.0-1.0 input range becomes 0.225-0.500
  smoothing_exp_average = 1.0 - smoothing_bottom_half; // invert input

  // PHOTONS knob is squared and applied here:
  FastLED.setBrightness((255 * MASTER_BRIGHTNESS) * (CONFIG.PHOTONS * CONFIG.PHOTONS));
}
