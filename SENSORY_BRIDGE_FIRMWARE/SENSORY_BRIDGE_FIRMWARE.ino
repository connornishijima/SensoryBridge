/*----------------------------------------

  ==      ===        ==  =======  ===      =====    ====       ===  ====  =======      ====       ===    ==       ====      ===        =
  =  ====  ==  ========   ======  ==  ====  ===  ==  ===  ====  ==   ==   =======  ===  ===  ====  ===  ===  ====  ==   ==   ==  =======
  =  ====  ==  ========    =====  ==  ====  ==  ====  ==  ====  ===  ==  ========  ====  ==  ====  ===  ===  ====  ==  ====  ==  =======
  ==  =======  ========  ==  ===  ===  =======  ====  ==  ===   ===  ==  ========  ===  ===  ===   ===  ===  ====  ==  ========  =======
  ====  =====      ====  ===  ==  =====  =====  ====  ==      ======    =========      ====      =====  ===  ====  ==  ========      ===
  ======  ===  ========  ====  =  =======  ===  ====  ==  ====  =====  ==========  ===  ===  ====  ===  ===  ====  ==  ===   ==  =======
  =  ====  ==  ========  =====    ==  ====  ==  ====  ==  ====  =====  ==========  ====  ==  ====  ===  ===  ====  ==  ====  ==  =======
  =  ====  ==  ========  ======   ==  ====  ===  ==  ===  ====  =====  ==========  ===  ===  ====  ===  ===  ====  ==   ==   ==  =======
  ==      ===        ==  =======  ===      =====    ====  ====  =====  ==========      ====  ====  ==    ==       ====      ===        =

  Welcome to the Sensory Bridge firmware files!
  
  If the above ASCII art is not showing "SENSORY BRIDGE" correctly in
  the negative space, you're probably on Arduino 2.0.x. This means
  you'll need to make a few changes for this firmware to work.

  1. DISABLE DEFAULT COMPILATION WARNING BEHAVIOR --------------------

    (This will make the firmware actually usable)

  - In the Arduino 2.0.x IDE, click "File > Preferences"
  - Set "Compiler Warnings" to "Default"
  - This is because some dependencies below trigger ignorable warnings
  - Arduino IDE 2.0.0 treats all warnings as errors by default.

  2. DISABLE WORD WRAPPING -------------------------------------------

    (This will guarantee code and docs more readable)

  - In the Arduino 2.0.x IDE, hit F1
  - In the textbox that appears, type "wrap"
  - You'll see a result for "View - Toggle Word Wrap"
  - Click this item
  - Repeat at least once until buggy word-wrapping is actually disabled

---------------------------------------------------------------------*/

#define FIRMWARE_VERSION "2.0.0" // Try "V" on the Serial port for this!

// Lightshow modes by name
enum lightshow_modes {
  DUET_MODE, // -------- FFT with linear scaling
  DUET_MODE_LOG, // ---- FFT with log scaling
  BLOOM_MODE, // ------- Slow Bloom Mode
  BLOOM_MODE_FAST, // -- Fast Bloom Mode
  WAVEFORM_MODE, // ---- Waveform is shown using LED brightness
  VU_MODE, // ---------- Not a real VU for any measurement sake, just a dance-y LED bar
  VU_MODE_DOT, // ------ Alternate VU display mode - dot with motion blur

  NUM_MODES  // used to know the length of this list if it changes
};

// External dependencies --------------------------------------------------------
#include <WiFi.h> // ------ Needed for Station Mode
#include <esp_now.h> // --- P2P wireless communication library
#include <FastLED.h> // --- Handles LED color data and display
#include <FS.h> // -------- Filesystem functions
#include <LittleFS.h> // -- LittleFS implementation
#include <Ticker.h> // ---- Scheduled tasks library

// Include Sensory Bridge firmware files ----------------------------------------
#include "ESP32_fft.h" // -------- Fast time to frequency domain conversion
#include "hardware_config.h" // -- LED strip length
#include "constants.h" // -------- Global constants
#include "globals.h" // ---------- Global variables
#include "bridge_fs.h" // -------- Filesystem access (save/load configuration)
#include "led_utilities.h" // ---- LED color/transform utility functions
#include "i2s.h" // -------------- I2S Microphone audio capture
#include "fft_parser.h" // ------- Conversion to (and post-processing) of FFT data
#include "noise_cal.h" // -------- Background noise removal
#include "p2p.h" // -------------- Sensory Sync handling
#include "sweet_spot.h" // ------- Drive sweet spot LEDs
#include "buttons.h" // ---------- Watch the status of buttons
#include "knobs.h" // ------------ Watch the status of knobs...
#include "serial_menu.h" // ------ Watch the Serial port... *sigh*
#include "system.h" // ----------- Watch how fast I can check if settings were updated... yada yada..
#include "lightshow_modes.h" // -- FINALLY, the FUN STUFF!

void setup() {
  init_system();  // (system.h) Initialize all hardware and arrays
}

void loop() {
  uint32_t t_now = millis();  // Timestamp for this loop, used by some core functions

  function_id = 0;  // These are for debug_function_timing(), to see what functions take up the most time
  check_knobs(t_now); // (knobs.h)
  // Check if the knobs have changed

  function_id = 1;
  check_buttons(t_now); // (buttons.h)
  // Check if the buttons have changed

  function_id = 2;
  check_settings(t_now); // (settings.h)
  // Check if the settings have changed

  function_id = 3;
  check_serial(t_now); // (serial_menu.h)
  // Check if UART commands are available

  function_id = 4;
  run_p2p(); // (p2p.h)
  // Process P2P network packets to synchronize units

  function_id = 5;
  capture_audio(); // (i2s.h)
  // Capture a frame of I2S audio

  function_id = 6;
  process_fft(); // (fft_parser.h)
  // Execute FFT and post-process

  function_id = 7;
  fft_lookahead_smoothing(); // (fft_parser.h)
  // Peek at upcoming frames to study/prevent flickering

  function_id = 8;
  run_noise_cal(t_now); // (noise_cal.h)
  // Run the noise calibration, if in that state

  function_id = 9;
  run_leds(); // (this file, below)
  // Run the lightshow functions

  function_id = 10;
  run_sweet_spot(); // (sweet_spot.h)
  // Run the Sweet Spot LEDs

#ifdef DEBUG_MODE
  function_id = 11;
  debug_function_timing(t_now); // (system.h)
  // Print how many times the functions were seen running in a 30 second window
#endif
}

void run_leds() {
  if (collecting_ambient_noise == false) { // If noise cal is running, let that control LEDs instead
    if (CONFIG.LIGHTSHOW_MODE == DUET_MODE) {
      duet_mode(); // (lightshow_modes.h) FFT with linear scaling
    }
    if (CONFIG.LIGHTSHOW_MODE == DUET_MODE_LOG) {
      duet_mode(); // (lightshow_modes.h) FFT with log scaling
    } else if (CONFIG.LIGHTSHOW_MODE == BLOOM_MODE) {
      bloom_mode(false); // (lightshow_modes.h) Slow Bloom Mode
    } else if (CONFIG.LIGHTSHOW_MODE == BLOOM_MODE_FAST) {
      bloom_mode(true); // (lightshow_modes.h) Fast Bloom Mode
    } else if (CONFIG.LIGHTSHOW_MODE == WAVEFORM_MODE) {
      waveform_mode(); // (lightshow_modes.h) Waveform is shown using LED brightness
    } else if (CONFIG.LIGHTSHOW_MODE == VU_MODE) {
      vu_mode(false); // (lightshow_modes.h) Not a real VU for any measurement sake, just a dance-y LED bar
    } else if (CONFIG.LIGHTSHOW_MODE == VU_MODE_DOT) {
      vu_mode(true); // (lightshow_modes.h) Alternate VU display mode - dot with motion blur
    }

    if (CONFIG.MIRROR_ENABLED) {  // Mirrored Mode logic
      if (CONFIG.LIGHTSHOW_MODE != BLOOM_MODE && CONFIG.LIGHTSHOW_MODE != BLOOM_MODE_FAST) {
        // all modes but BLOOM are scaled before mirroring
        scale_image_to_half(); // (led_utilities.h) Image is now 50% height
        shift_leds_up(64);     // (led_utilities.h) Move image up one half
      } else {
        shift_leds_up(64);  // (led_utilities.h) Move image up one half
        fade_edge(false);   // (led_utilities.h) Vignette top edge
      }

      mirror_image_downwards(); // (led_utilities.h) Mirror downwards
    } else {
      if (CONFIG.LIGHTSHOW_MODE == BLOOM_MODE || CONFIG.LIGHTSHOW_MODE == BLOOM_MODE_FAST) {
        // Bloom Mode uses vignetting at a single edge when not mirrored
        fade_edge(false); // (led_utilities.h)
      }
    }

    show_leds(); // (led_utilities.h)
  }
}