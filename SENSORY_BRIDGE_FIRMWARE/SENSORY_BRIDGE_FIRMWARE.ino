#define FIRMWARE_VERSION "1.2.0"

// Include dependencies
#include <FastLED.h>
#include <FS.h>
#include <LittleFS.h>
#include "cdcusb.h"

// CDC class definition
CDCusb USBSerial;

// Used to reference modes by name in code
enum lightshow_modes {
  DUET_MODE,
  DUET_MODE_LOG,
  BLOOM_MODE,
  WAVEFORM_MODE,
  VU_MODE,

  NUM_MODES // used to know the length of this list
};

// Include project files
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
  init_bridge(); // Initialize LEDs, USB, buttons, etc. (system.h)
}

void loop() {
  // Check if inputs changed (system.h)
  check_knobs();
  check_buttons();

  // Check we're hitting the Sweet Spot (system.h)
  check_sweet_spot();

  // Check for commands over Serial (system.h)
  check_serial();

  // Check if settings need to be written to LittleFS (system.h)
  check_settings();

  // Use I2S to capture the audio buffer (i2s.h)
  capture_audio();

  // Run the FFT on the audio buffer (fft.h)
  process_fft();

  // Depending on the current lightshow mode,
  // run a different function for the LEDs
  // (lightshow_modes.h)
  if (LIGHTSHOW_MODE == DUET_MODE) {
    duet_mode(); // FFT Display
  }
  else if (LIGHTSHOW_MODE == DUET_MODE_LOG) {
    duet_mode(); // FFT Display without linear scaling
  }
  else if (LIGHTSHOW_MODE == BLOOM_MODE) {
    bloom_mode(); // Amplitude is scrolled temporally
  }
  else if (LIGHTSHOW_MODE == WAVEFORM_MODE) {
    waveform_mode(); // Shows the current waveform
  }
  else if (LIGHTSHOW_MODE == VU_MODE) {
    vu_mode(); // VU Meter mode
  }

  // Mirroring is enabled by holding MODE for more than 250ms
  if (MIRROR_ENABLED) {

    if (LIGHTSHOW_MODE != BLOOM_MODE) { // Bloom mode isn't scaled
      interpolate_scale_leds(0.5); // (led_utilities.h)
    }
    shift_leds_up(64); // (led_utilities.h)
    mirror_image_downwards(); // (led_utilities.h)
    
    if (LIGHTSHOW_MODE == BLOOM_MODE) {
      fade_edge(true); // Bloom mode has darkened edges IF mirrored
    }
  }
  else{
    if (LIGHTSHOW_MODE == BLOOM_MODE) {
      fade_edge(false); // (led_utilities.h)
    }
  }

  if (!collecting_ambient_noise) { // Only shown if not calibrating noise
    show_leds(); // Render LED data (led_utilities.h)
  }
  if (debug_mode) {
    log_fps(); // (system.h)
  }
}
