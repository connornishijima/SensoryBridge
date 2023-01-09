/*------------------------------------------------------------------------------------------------------------------------------------

   ======   ========  ==       ==   ======     ====    =======   ==    ==       ======    =======   ====  =======    ======   ========
  ==    ==  ==        ===      ==  ==    ==   ==  ==   ==    ==  ===  ===       ==   ==   ==    ==   ==   ==    ==  ===  ===  ==
  ==    ==  ==        ====     ==  ==    ==  ==    ==  ==    ==   ==  ==        ==    ==  ==    ==   ==   ==    ==  ==    ==  ==
   ==       ==        ==  ==   ==   ==       ==    ==  ==   ===   ==  ==        ==   ==   ==   ===   ==   ==    ==  ==        ==
     ==     ======    ==   ==  ==     ==     ==    ==  ======      ====         ======    ======     ==   ==    ==  ==        ======
       ==   ==        ==    == ==       ==   ==    ==  ==    ==     ==          ==   ==   ==    ==   ==   ==    ==  ==   ===  ==
  ==    ==  ==        ==     ====  ==    ==  ==    ==  ==    ==     ==          ==    ==  ==    ==   ==   ==    ==  ==    ==  ==
  ==    ==  ==        ==      ===  ==    ==   ==  ==   ==    ==     ==          ==   ==   ==    ==   ==   ==    ==  ===  ===  ==
   ======   ========  ==       ==   ======     ====    ==    ==     ==          ======    ==    ==  ====  =======    ======   ========

  by @lixielabs (2022 - 2023)

  ####################################################################

  Welcome to the Sensory Bridge firmware files!

  WARNING:
  
    If the above ASCII art is not showing "SENSORY BRIDGE" correctly,
    you're probably on Arduino 2.0.x. This means you'll need to make a
    few changes for this firmware to work.
    
  ####################################################################

  1. DISABLE WORD WRAPPING -------------------------------------------

    (This will guarantee code and docs are more readable)

  - In the Arduino 2.0.x IDE, hit F1
  - In the textbox that appears, type "wrap"
  - You'll see a result for "View - Toggle Word Wrap"
  - Click this item
  - Repeat at least once until buggy word-wrapping is actually disabled
  - This is a painful change that the IDE 1.x didn't do
      (In fact no other IDE I've ever used does this, ugh.)

  2. DISABLE DEFAULT COMPILATION WARNING BEHAVIOR --------------------

    (This will make the firmware actually usable)

  - In the Arduino 2.0.x IDE, click "File > Preferences"
  - Set "Compiler Warnings" to "Default"
  - This is because some dependencies below trigger ignorable warnings
  - Arduino IDE 2.0.0 treats all warnings as errors by default.
  - This is a *painful* change that the IDE 1.x didn't do

  Anyways, let's begin!

  ---------------------------------------------------------------------*/

#define FIRMWARE_VERSION "3.0.0" // Try "V" on the Serial port for this!

// Lightshow modes by name -----------------------------------------------------------
enum lightshow_modes {
  LIGHT_MODE_GDFT, // ------------- GDFT - Goertzel-based Discrete Fourier Transform
                   //                      (I made this name up. Saved you a search.)
  
  // LIGHT_MODE_GDFT_RETRO, // ------- GDFT with retro bulbs (TODO)
  // LIGHT_MODE_GDFT_CHROMAGRAM, // -- Chromagram of GDFT (TODO)
  // LIGHT_MODE_BLOOM, // ------------ Slow Bloom Mode (TODO)
  // LIGHT_MODE_BLOOM_FAST, // ------- Fast Bloom Mode (TODO)
  // LIGHT_MODE_WAVEFORM, // --------- Waveform is shown using LED brightness (TODO)
  // LIGHT_MODE_VU, // --------------- Not a real VU for any measurement sake, just a dance-y LED bar (TODO)
  // LIGHT_MODE_VU_DOT, // ----------- Alternate VU display mode - dot with motion blur (TODO)

  NUM_MODES  // used to know the length of this list if it changes in the future
};

// External dependencies -------------------------------------------------------------
#include <WiFi.h> // ------ Needed for Station Mode
#include <esp_now.h> // --- P2P wireless communication library (p2p.h below)
#include <FastLED.h> // --- Handles LED color data and display
#include <FS.h> // -------- Filesystem functions (bridge_fs.h below)
#include <LittleFS.h> // -- LittleFS implementation
#include <Ticker.h> // ---- Scheduled tasks library

// Include Sensory Bridge firmware files, sorted high to low, by boringness ;) -------
#include "strings.h" // ---------- Strings for printing
#include "user_config.h" // ------ LED strip length
#include "constants.h" // -------- Global constants
#include "globals.h" // ---------- Global variables
#include "bridge_fs.h" // -------- Filesystem access (save/load configuration)
#include "i2s_audio.h" // -------- I2S Microphone audio capture
#include "led_utilities.h" // ---- LED color/transform utility functions
#include "noise_cal.h" // -------- Background noise removal
#include "p2p.h" // -------------- Sensory Sync handling
#include "utilities.h" // -------- Misc. math and other functions
#include "buttons.h" // ---------- Watch the status of buttons
#include "knobs.h" // ------------ Watch the status of knobs...
#include "serial_menu.h" // ------ Watch the Serial port... *sigh* 
#include "system.h" // ----------- Watch how fast I can check if settings were updated... yada yada..
#include "GDFT.h" // ------------- Conversion to (and post-processing of) frequency data! (hey, something cool!)
#include "lightshow_modes.h" // -- FINALLY, the FUN STUFF!

// Setup, runs only one time ---------------------------------------------------------
void setup() {
  init_system(); // (system.h) Initialize all hardware and arrays
}

// Loop, runs forever after setup() --------------------------------------------------
void loop() {
  uint32_t t_now_us = micros();     // Timestamp for this loop, used by some core functions
  uint32_t t_now = t_now_us/1000.0; // Millisecond version

  function_id = 0;  // These are for debug_function_timing(), to see what functions take up the most time
  check_knobs(t_now); // (knobs.h)
  // Check if the knobs have changed

  function_id = 1;
  check_buttons(t_now); // (buttons.h)
  // Check if the buttons have changed

  function_id = 2;
  check_settings(t_now); // (system.h)
  // Check if the settings have changed

  function_id = 3;
  check_serial(t_now); // (serial_menu.h)
  // Check if UART commands are available

  function_id = 4;
  run_p2p(); // (p2p.h)
  // Process P2P network packets to synchronize units

  function_id = 5;
  acquire_sample_chunk(); // (i2s_audio.h)
  // Capture a frame of I2S audio (holy crap, FINALLY something about sound)

  function_id = 6;
  process_GDFT(); // (GDFT.h)
  // Execute GDFT and post-process
  // (If you're wondering about that weird acronym, check out the source file)

  function_id = 7;
  lookahead_smoothing(); // (GDFT.h)
  // Peek at upcoming frames to study/prevent flickering

  function_id = 8;
  render_leds(t_now_us); // (BELOW in this file)
  // Convert the data we found into a beautiful show

#if DEBUG_MODE == 1
  function_id = 31;
  debug_function_timing(t_now);
#endif
  
  yield(); // Otherwise the ESP32 will collapse into a black hole or something
}

// Run the lights! -------------------------------------------------------------------
// Based on the value of CONFIG.LIGHTSHOW_MODE, we call a
// different rendering function from lightshow_modes.h:
void render_leds(uint32_t t_now_us) {
  if (noise_complete == true) { // If we're not gathering ambient noise data
    if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_GDFT) {
      light_mode_gdft(); // (lightshow_modes.h) GDFT display
    }
  }

  if (CONFIG.MIRROR_ENABLED) { // Mirrored Mode logic
    scale_image_to_half();     // (led_utilities.h) Image is now 50% height
    shift_leds_up(64);         // (led_utilities.h) Move image up one half
    mirror_image_downwards();  // (led_utilities.h) Mirror downwards
  }

  show_leds(); // This sends final RGB data to the LEDS (led_utilities.h)
  log_fps(t_now_us); // (system.h)
}
