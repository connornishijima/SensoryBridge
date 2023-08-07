/*------------------------------------------------------------------------------------------------------------------------------------

   ######   ########  ##       ##   ######     ####    #######   ##    ##       ######    #######   ####  #######    ######   ########
  ##    ##  ##        ###      ##  ##    ##   ##  ##   ##    ##  ###  ###       ##   ##   ##    ##   ##   ##    ##  ###  ###  ##
  ##    ##  ##        ####     ##  ##    ##  ##    ##  ##    ##   ##  ##        ##    ##  ##    ##   ##   ##    ##  ##    ##  ##
   ##       ##        ##  ##   ##   ##       ##    ##  ##   ###   ##  ##        ##   ##   ##   ###   ##   ##    ##  ##        ##
     ##     ######    ##   ##  ##     ##     ##    ##  ######      ####         ######    ######     ##   ##    ##  ##        ######
       ##   ##        ##    ## ##       ##   ##    ##  ##    ##     ##          ##   ##   ##    ##   ##   ##    ##  ##   ###  ##
  ##    ##  ##        ##     ####  ##    ##  ##    ##  ##    ##     ##          ##    ##  ##    ##   ##   ##    ##  ##    ##  ##
  ##    ##  ##        ##      ###  ##    ##   ##  ##   ##    ##     ##          ##   ##   ##    ##   ##   ##    ##  ###  ###  ##
   ######   ########  ##       ##   ######     ####    ##    ##     ##          ######    ##    ##  ####  #######    ######   ########

  by @lixielabs (2022 - 2023)

  ####################################################################

  Welcome to the Sensory Bridge firmware files!

  WARNING:

    If the above ASCII art is not showing "SENSORY BRIDGE" correctly,
    you're probably on < Arduino 2.1.0. This means you'll need to make a
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

#define FIRMWARE_VERSION 40000  // Try "V" on the Serial port for this!
//                       MmmPP     M = Major version, m = Minor version, P = Patch version
//                                 (i.e 3.5.4 would be 30504)


// SensoryBridge will run on the LumosStick and the LumosRing. To use either, uncomment only one of the following lines
#define LUMOSSTICK  // controls hardware configuration for the LumosStick
//#define LUMOSRING   // controls hardware configuration for the LumosRing


// Lightshow modes by name -----------------------------------------------------------
enum lightshow_modes {
  LIGHT_MODE_GDFT,  // ------------- GDFT - Goertzel-based Discrete Fourier Transform
  //                                 (I made this name up. Saved you a search.)
  LIGHT_MODE_GDFT_CHROMAGRAM,       // -- Chromagram of GDFT
  LIGHT_MODE_GDFT_CHROMAGRAM_DOTS,  // -- Chromagram of GDFT
  LIGHT_MODE_BLOOM,                 // -- Bloom Mode
  LIGHT_MODE_VU_DOT,                // -- Not a real VU for any measurement sake, just a dance-y LED show
  LIGHT_MODE_KALEIDOSCOPE,          // -- Three color channels 2D Perlin noise affected by the onsets of low, mid and high pitches

  NUM_MODES  // used to know the length of this list if it changes in the future
};

// External dependencies -------------------------------------------------------------
#include <WiFi.h>         // Needed for Station Mode
#include <esp_now.h>      // P2P wireless communication library (p2p.h below)
#include <esp_random.h>   // RNG Functions
#include <FastLED.h>      // Handles LED color data and display
#include <FS.h>           // Filesystem functions (bridge_fs.h below)
#include <LittleFS.h>     // LittleFS implementation
#include <Ticker.h>       // Scheduled tasks library
#include <USB.h>          // USB Connection handling
#include <FirmwareMSC.h>  // Allows firmware updates via USB MSC
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

// Include Sensory Bridge firmware files, sorted high to low, by boringness ;) -------
#include "strings.h"          // Strings for printing
#include "user_config.h"      // Nothing for now
#include "constants.h"        // Global constants
#include "globals.h"          // Global variables
#include "presets.h"          // Configuration presets by name
#include "bridge_fs.h"        // Filesystem access (save/load configuration)
#include "utilities.h"        // Misc. math and other functions
#include "i2s_audio.h"        // I2S Microphone audio capture
#include "led_utilities.h"    // LED color/transform utility functions
#include "noise_cal.h"        // Background noise removal
#include "p2p.h"              // Sensory Sync handling
#include "buttons.h"          // Watch the status of buttons
#include "knobs.h"            // Watch the status of knobs...
#include "serial_menu.h"      // Watch the Serial port... *sigh*
#include "system.h"           // Watch how fast I can check if settings were updated... yada yada..
#include "GDFT.h"             // Conversion to (and post-processing of) frequency data! (hey, something cool!)
#include "lightshow_modes.h"  // --- FINALLY, the FUN STUFF!

// Setup, runs only one time ---------------------------------------------------------
void setup() {
  init_system();  // (system.h) Initialize all hardware and arrays

  // Create thread specifically for LED updates
  xTaskCreatePinnedToCore(led_thread, "led_task", 4096, NULL, tskIDLE_PRIORITY + 1, &led_task, 1);
}

// Loop, runs forever after setup() --------------------------------------------------
void loop() {
  uint32_t t_now_us = micros();        // Timestamp for this loop, used by some core functions
  uint32_t t_now = t_now_us / 1000.0;  // Millisecond version

  function_id = 0;     // These are for debug_function_timing() in system.h to see what functions take up the most time
  check_knobs(t_now);  // (knobs.h)
  // Check if the knobs have changed

  function_id = 1;
  check_buttons(t_now);  // (buttons.h)
  // Check if the buttons have changed

  function_id = 2;
  check_settings(t_now);  // (system.h)
  // Check if the settings have changed

  function_id = 3;
  check_serial(t_now);  // (serial_menu.h)
  // Check if UART commands are available

  function_id = 4;
  run_p2p();  // (p2p.h)
  // Process P2P network packets to synchronize units

  function_id = 5;
  acquire_sample_chunk(t_now);  // (i2s_audio.h)
  // Capture a frame of I2S audio (holy crap, FINALLY something about sound)

  function_id = 6;
  run_sweet_spot();  // (led_utilities.h)
  // Based on the current audio volume, alter the Sweet Spot indicator LEDs

  // Calculates audio loudness (VU) using RMS, adjusting for noise floor based on calibration
  calculate_vu();

  function_id = 7;
  process_GDFT();  // (GDFT.h)
  // Execute GDFT and post-process
  // (If you're wondering about that weird acronym, check out the source file)

  // Watches the rate of change in the Goertzel bins to guide decisions for auto-color shifting
  calculate_novelty(t_now);

  if (CONFIG.AUTO_COLOR_SHIFT == true) {  // Automatically cycle color based on density of positive spectral changes
    // Use the "novelty" findings of the above function to affect color shifting when auto-color shifts are enabled
    process_color_shift();
  } else {
    hue_position = 0;
    hue_shifting_mix = -0.35;
  }

  function_id = 8;
  //lookahead_smoothing();  // (GDFT.h)
  // Peek at upcoming frames to study/prevent flickering

  function_id = 8;
  log_fps(t_now_us);  // (system.h)
  // Log the audio system FPS

  if (debug_mode) {
    function_id = 31;
    debug_function_timing(t_now);
  }

  yield();  // Otherwise the ESP32 will collapse into a black hole or something
}

// Run the lights in their own thread! -------------------------------------------------------------
void led_thread(void* arg) {
  while (true) {
    if (led_thread_halt == false) {                                             // If we're not gathering ambient noise data
      if (mode_transition_queued == true || noise_transition_queued == true) {  // If transition queued
        run_transition_fade();                                                  // (led_utilities.h) Fade to black between modes
      }

      get_smooth_spectrogram();
      make_smooth_chromagram();

      // Based on the value of CONFIG.LIGHTSHOW_MODE, we call a
      // different rendering function from lightshow_modes.h:

      if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_GDFT) {
        light_mode_gdft();  // (lightshow_modes.h) GDFT spectrogram display
      } else if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_GDFT_CHROMAGRAM) {
        light_mode_chromagram_gradient();  //light_mode_chromagram_gradient();  // (lightshow_modes.h) GDFT chromagram display
      } else if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_GDFT_CHROMAGRAM_DOTS) {
        light_mode_chromagram_dots();  //light_mode_chromagram_dots();  // (lightshow_modes.h) GDFT chromagram display
      } else if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_BLOOM) {
        light_mode_bloom();  // (lightshow_modes.h) Bloom Mode display
      } else if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_VU_DOT) {
        light_mode_vu_dot();  // (lightshow_modes.h) VU Mode display (dot version)
      } else if (CONFIG.LIGHTSHOW_MODE == LIGHT_MODE_KALEIDOSCOPE) {
        light_mode_kaleidoscope();  // (lightshow_modes.h) Kaleidoscope Mode display
      }

      if (CONFIG.PRISM_COUNT > 0) {
        apply_prism_effect(CONFIG.PRISM_COUNT, 0.25);
      }

      // Render bulb filter
      if (CONFIG.BULB_OPACITY > 0.00) {
        render_bulb_cover();
      }

      // If forcing monochromatic incandescent output
      if (CONFIG.INCANDESCENT_MODE == true) {
        for (uint8_t i = 0; i < NATIVE_RESOLUTION; i++) {
          leds_16[i] = adjust_hue_and_saturation(leds_16[i], 0.05, 0.95);
        }
      }

      if (CONFIG.MIRROR_ENABLED == false) {  // Mirroring logic
        unmirror();
      }

      show_leds();  // This sends final RGB data to the LEDS (led_utilities.h)
      LED_FPS = FastLED.getFPS();
    }
    else{
      vTaskDelay(0);
    }

    if (CONFIG.LED_TYPE == LED_NEOPIXEL) {
      //vTaskDelay(1); // delay for 1ms to avoid hogging the CPU
    } else if (CONFIG.LED_TYPE == LED_DOTSTAR) {  // More delay to compensate for faster LEDs
      //vTaskDelay(3);                              // delay for 3ms to avoid hogging the CPU
    }
  }
}