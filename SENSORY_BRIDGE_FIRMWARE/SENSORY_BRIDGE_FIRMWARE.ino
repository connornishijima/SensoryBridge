#define FIRMWARE_VERSION "1.1.0"

#include <FastLED.h>
#include <FS.h>
#include <LittleFS.h>
#include "cdcusb.h"

CDCusb USBSerial;

enum lightshow_modes {
  DUET_MODE,
  DUET_MODE_LOG,
  BLOOM_MODE,
  WAVEFORM_MODE,
  VU_MODE,

  NUM_MODES // used to know the length of this list
};

#include "ESP32_fft.h"
#include "configuration.h"
#include "constants.h"
#include "globals.h"
#include "USB_handler.h"
#include "led_utilities.h"
#include "noise_cal.h"
#include "i2s.h"
#include "system.h"
#include "fft.h"
#include "lightshow_modes.h"

void setup() {
  init_bridge();
}

void loop() {
  check_knobs();
  check_buttons();
  check_sweet_spot();
  check_serial();
  check_settings();

  capture_audio();
  process_fft();

  if (LIGHTSHOW_MODE == DUET_MODE) {
    duet_mode(false);
  }
  else if (LIGHTSHOW_MODE == DUET_MODE_LOG) {
    duet_mode(false);
  }
  else if (LIGHTSHOW_MODE == BLOOM_MODE) {
    bloom_mode();
  }
  else if (LIGHTSHOW_MODE == WAVEFORM_MODE) {
    waveform_mode();
  }
  else if (LIGHTSHOW_MODE == VU_MODE) {
    vu_mode();
  }

  if (MIRROR_ENABLED) {    
    if (LIGHTSHOW_MODE != BLOOM_MODE) {
      interpolate_scale_leds(0.5);
    }
    shift_leds_up(64);
    mirror_image_downwards();
    
    if (LIGHTSHOW_MODE == BLOOM_MODE) {
      fade_edge(true);
    }
  }
  else{
    if (LIGHTSHOW_MODE == BLOOM_MODE) {
      fade_edge(false);
    }
  }

  if (!collecting_ambient_noise) {
    show_leds();
  }
  if (debug_mode) {
    log_fps();
  }
}
