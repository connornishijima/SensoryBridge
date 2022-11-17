/*----------------------------------------
  Sensory Bridge KNOB FUNCTIONS
----------------------------------------*/

void check_knobs(uint32_t t_now) {
  static uint32_t iter = 0;
  const uint8_t averaging_steps = 4;
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

  if (iter % 10 == 0) {
    float photons_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      photons_sum += (analogRead(PHOTONS_PIN) / 8192.0);
    }
    PHOTONS_TARGET = (1.0 - photons_sum / float(averaging_steps));

    float chroma_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      chroma_sum += (analogRead(CHROMA_PIN) / 8192.0);
    }
    CHROMA_TARGET = (1.0 - chroma_sum / float(averaging_steps));

    float mood_sum = 0;
    for (uint8_t i = 0; i < averaging_steps; i++) {
      mood_sum += (1.0 - (analogRead(MOOD_PIN) / 8192.0));
    }
    MOOD_TARGET = mood_sum / float(averaging_steps);
  }

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

  if (CONFIG.IS_MAIN_UNIT || main_override) {
    CONFIG.PHOTONS = PHOTONS_OUTPUT;
    CONFIG.CHROMA = CHROMA_OUTPUT;
    CONFIG.MOOD = MOOD_OUTPUT;
  } else {
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

  if(t_now >= 1000){
    if(MASTER_BRIGHTHNESS < 1.0){
      MASTER_BRIGHTHNESS += 0.01;
    }
    if(MASTER_BRIGHTHNESS > 1.0){
      MASTER_BRIGHTHNESS = 1.00;
    }
  }

  FastLED.setBrightness((255 * MASTER_BRIGHTHNESS) * (CONFIG.PHOTONS * CONFIG.PHOTONS));
}