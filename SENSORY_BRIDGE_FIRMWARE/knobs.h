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
    PHOTONS_TARGET = (1.0 - (analogRead(PHOTONS_PIN) / 4095.0));
    CHROMA_TARGET  = (1.0 - (analogRead(CHROMA_PIN)  / 4095.0));
    MOOD_TARGET    = (1.0 - (analogRead(MOOD_PIN)    / 4095.0));
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
    if(MASTER_BRIGHTHNESS < 1.0){
      MASTER_BRIGHTHNESS += 0.01;
    }
    if(MASTER_BRIGHTHNESS > 1.0){
      MASTER_BRIGHTHNESS = 1.00;
    }
  }

  // PHOTONS knob is squared and applied here:
  FastLED.setBrightness((255 * MASTER_BRIGHTHNESS) * (CONFIG.PHOTONS * CONFIG.PHOTONS));
}
