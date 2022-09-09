#define FIRMWARE_VERSION "1.0.0"

#include <FastLED.h>
#include <FS.h>
#include <LittleFS.h>

enum lightshow_modes {
  DUET_MODE,
  DUET_MODE_MIRROR,
  DUET_MODE_INVERTED,
  DUET_MODE_INVERTED_MIRROR,
  BLOOM_MODE,
  //VELOCITY_MODE,
  //TEMPO_SINE_MODE,
  //TEMPO_ORBS_MODE,

  NUM_MODES // used to know the length of this list
};

#include "ESP32_fft.h"
#include "configuration.h"
#include "constants.h"
#include "globals.h"
#include "noise_cal.h"
#include "i2s.h"
#include "system.h"
#include "fft.h"
#include "led_utilities.h"
#include "lightshow_modes.h"

void setup() {
  init_bridge();
}

void loop() {
  check_knobs();
  check_buttons();
  check_sweet_spot();
  check_serial();

  capture_audio();
  process_fft();

  if (LIGHTSHOW_MODE == DUET_MODE) {
    duet_mode(false, incandescent_mode);
    /*
    flip_image();
    interpolate_scale_leds(1.196261);
    flip_image();
    */
  }
  else if (LIGHTSHOW_MODE == DUET_MODE_MIRROR) {
    duet_mode(false, incandescent_mode);
    interpolate_scale_leds(0.5);
    shift_leds_up(64);
    mirror_image_downwards();
  }
  else if (LIGHTSHOW_MODE == DUET_MODE_INVERTED) {
    duet_mode(true, incandescent_mode);
  }
  else if (LIGHTSHOW_MODE == DUET_MODE_INVERTED_MIRROR) {
    duet_mode(true, incandescent_mode);
    interpolate_scale_leds(0.5);
    shift_leds_up(64);
    mirror_image_downwards();
  }
  /*
  else if (LIGHTSHOW_MODE == VELOCITY_MODE) {
    velocity_mode(true);
  }
  else if (LIGHTSHOW_MODE == TEMPO_SINE_MODE) {
    float vel = velocity_mode(false); // Needed for BPM
    tempo_sine_mode(vel);
  }
  else if (LIGHTSHOW_MODE == TEMPO_ORBS_MODE) {
    float vel = velocity_mode(false); // Needed for BPM
    tempo_orbs_mode(vel);

  }
  */
  else if (LIGHTSHOW_MODE == BLOOM_MODE) {
    bloom_mode(incandescent_mode);
  }

  show_leds();
  if(debug_mode){
    log_fps();
  }
}
