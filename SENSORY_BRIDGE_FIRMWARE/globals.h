// ------------------------------------------------------------
// Configuration structure ------------------------------------

struct conf {  
  // Synced values
  float   PHOTONS;
  float   CHROMA;
  float   MOOD;
  uint8_t LIGHTSHOW_MODE;
  bool    MIRROR_ENABLED;

  // Private values
  uint32_t SAMPLE_RATE;
  uint8_t  NOTE_OFFSET;
  uint8_t  SQUARE_ITER;
  uint8_t  LED_TYPE;
  uint16_t LED_COUNT;
  uint16_t LED_COLOR_ORDER;
  bool     LED_INTERPOLATION;
  uint16_t SAMPLES_PER_CHUNK;
  float    SENSITIVITY;
  bool     BOOT_ANIMATION;
  uint32_t SWEET_SPOT_MIN_LEVEL;
  uint32_t SWEET_SPOT_MAX_LEVEL;
  int32_t  DC_OFFSET;
  uint8_t  CHROMAGRAM_RANGE;
  bool     STANDBY_DIMMING;
  bool     REVERSE_ORDER;
  bool     IS_MAIN_UNIT;
  uint32_t MAX_CURRENT_MA;
  bool     TEMPORAL_DITHERING;
  bool     AUTO_COLOR_SHIFT;
  float    INCANDESCENT_FILTER;
  bool     INCANDESCENT_MODE;
  float    BULB_OPACITY;
  float    SATURATION;
  uint8_t  PRISM_COUNT;
  bool     BASE_COAT;
  float    VU_LEVEL_FLOOR;
  uint8_t  LUMOS_ORDER;   // used by LumosStick to switch between rendering rows first or columns first
};

// ------------------------------------------------------------
// Defaults of the CONFIG struct (factory_reset values) -------

conf CONFIG = {
  // Synced values
  1.00, // PHOTONS
  0.00, // CHROMA
  0.05, // MOOD
  LIGHT_MODE_GDFT, // LIGHTSHOW_MODE
  true,           // MIRROR_ENABLED

  // Private values
  DEFAULT_SAMPLE_RATE, // SAMPLE_RATE
  12,                  // NOTE_OFFSET
  1,                   // SQUARE_ITER
  LED_NEOPIXEL,        // LED_TYPE
  TOTAL_LED_COUNT,     // LED_COUNT
  GRB,                 // LED_COLOR_ORDER
  true,                // LED_INTERPOLATION
  96,                  // SAMPLES_PER_CHUNK
  1.0,                 // SENSITIVITY
  true,                // BOOT_ANIMATION
  750,                 // SWEET_SPOT_MIN_LEVEL
  30000,               // SWEET_SPOT_MAX_LEVEL
  0,                   // DC_OFFSET
  60,                  // CHROMAGRAM_RANGE
  true,                // STANDBY_DIMMING
  false,               // REVERSE_ORDER
  false,               // IS_MAIN_UNIT
  1500,                // MAX_CURRENT_MA
  true,                // TEMPORAL_DITHERING
  false,               // AUTO_COLOR_SHIFT
  0.50,                // INCANDESCENT_FILTER
  false,               // INCANDESCENT_MODE
  0.00,                // BULB_OPACITY
  1.00,                // SATURATION
  0,                   // PRISM_COUNT
  false,               // BASE_COAT
  0.00,                // VU_LEVEL_FLOOR
  1,                   // LUMOS_ORDER
};

conf CONFIG_DEFAULTS; // Used for resetting to default values at runtime

char mode_names[NUM_MODES*32] = { 0 };

// ------------------------------------------------------------
// Goertzel structure (generated in system.h) -----------------

struct freq {
  float    target_freq;
  int32_t  coeff_q14;

  uint16_t block_size;
  float    block_size_recip;
  uint8_t  zone;
  
  float a_weighting_ratio;
  float window_mult;
};
freq frequencies[NUM_FREQS];

// ------------------------------------------------------------
// Hann window lookup table (generated in system.h) -----------

int16_t window_lookup[4096] = { 0 };

// ------------------------------------------------------------
// A-weighting lookup table (parsed in system.h) --------------

float a_weight_table[13][2] = {
  { 10,    -70.4 },  // hz, db
  { 20,    -50.5 },
  { 40,    -34.6 },
  { 80,    -22.5 },
  { 160,   -13.4 },
  { 315,    -6.6 },
  { 630,    -1.9 },
  { 1000,    0.0 },
  { 1250,    0.6 },
  { 2500,    1.3 },
  { 5000,    0.5 },
  { 10000,  -2.5 },
  { 20000,  -9.3 }
};

// ------------------------------------------------------------
// Spectrograms (GDFT.h) --------------------------------------

SQ15x16 spectrogram[NUM_FREQS] = { 0.0 };
SQ15x16 spectrogram_smooth[NUM_FREQS] = { 0.0 };
SQ15x16 chromagram_smooth[12] = { 0.0 };

SQ15x16 spectral_history[SPECTRAL_HISTORY_LENGTH][NUM_FREQS];
SQ15x16 novelty_curve[SPECTRAL_HISTORY_LENGTH] = { 0.0 };

uint8_t spectral_history_index = 0;

float note_spectrogram[NUM_FREQS] = {0};
float note_spectrogram_smooth[NUM_FREQS] = {0};
float note_spectrogram_smooth_frame_blending[NUM_FREQS] = {0};
float note_spectrogram_long_term[NUM_FREQS] = {0};
float note_chromagram[12]  = {0};
float chromagram_max_val = 0.0;
float chromagram_bass_max_val = 0.0;

float smoothing_follower    = 0.0;
float smoothing_exp_average = 0.0;

SQ15x16 chroma_val = 1.0;
bool chromatic_mode = true;

// ------------------------------------------------------------
// Audio samples (i2s_audio.h) --------------------------------

int32_t i2s_samples_raw[1024]                = { 0 };
short   sample_window[SAMPLE_HISTORY_LENGTH] = { 0 };
short   waveform[1024]                       = { 0 };
SQ15x16 waveform_fixed_point[1024]           = { 0 };
short   waveform_history[4][1024]            = { 0 };
uint8_t waveform_history_index = 0;
float   max_waveform_val_raw = 0.0;
float   max_waveform_val = 0.0;
float   max_waveform_val_follower = 0.0;
float   waveform_peak_scaled = 0.0;
int32_t dc_offset_sum = 0;
bool    silence = false;
float   silent_scale = 1.0;
float   current_punch = 0.0;

// ------------------------------------------------------------
// Sweet Spot (i2s_audio.h, led_utilities.h) ------------------

float sweet_spot_state = 0;
float sweet_spot_state_follower = 0;
float sweet_spot_min_temp = 0;

// ------------------------------------------------------------
// Noise calibration (noise_cal.h) ----------------------------

bool     noise_complete = true;
SQ15x16  noise_samples[NUM_FREQS] = { 1 };
uint16_t noise_iterations = 0;

// ------------------------------------------------------------
// Display buffers (led_utilities.h) --------------------------

/*
CRGB leds[128];
CRGB leds_frame_blending[128];
CRGB leds_fx[128];
CRGB leds_temp[128];
CRGB leds_last[128];
CRGB leds_aux [128];
CRGB leds_fade[128];
*/

// WARNING !! something is running over the end if one of these objects; crashing in intro_animation()
CRGB16  leds_16[NATIVE_RESOLUTION+1];
CRGB16  leds_16_prev[NATIVE_RESOLUTION+1];
CRGB16  leds_16_fx[NATIVE_RESOLUTION+1];
CRGB16  leds_16_fx_2[NATIVE_RESOLUTION+1];
CRGB16  leds_16_temp[NATIVE_RESOLUTION+1];
CRGB16  leds_16_ui[NATIVE_RESOLUTION+1];

SQ15x16 ui_mask[NATIVE_RESOLUTION+1];
SQ15x16 ui_mask_height = 0.0;

CRGB16 *leds_scaled;
CRGB *leds_out;

SQ15x16 hue_shift = 0.0; // Used in auto color cycling

uint8_t dither_step = 0;
bool led_thread_halt = false;
TaskHandle_t led_task;

// ------------------------------------------------------------
// Benchmarking (system.h) ------------------------------------

Ticker cpu_usage;
volatile uint16_t function_id = 0;
volatile uint16_t function_hits[32] = {0};
float SYSTEM_FPS = 0.0;
float LED_FPS    = 0.0;

// ------------------------------------------------------------
// SensorySync P2P network (p2p.h) ----------------------------

bool     main_override = true;
uint32_t last_rx_time = 0;

// ------------------------------------------------------------
// Buttons (buttons.h) ----------------------------------------

// TODO: Similar structs for knobs
struct button{
  uint8_t pin = 0;
  uint32_t last_down = 0;
  uint32_t last_up = 0;
  bool pressed = false;
};

button noise_button;
button mode_button;

bool    mode_transition_queued  = false;
int16_t mode_destination = -1;

bool    noise_transition_queued = false;

// ------------------------------------------------------------
// Settings tracking (system.h) -------------------------------

uint32_t next_save_time = 0;
bool     settings_updated = false;

// ------------------------------------------------------------
// Serial buffer (serial_menu.h) ------------------------------

char    command_buf[128] = {0};
uint8_t command_buf_index = 0;

bool stream_audio = false;
bool stream_fps = false;
bool stream_max_mags = false;
bool stream_max_mags_followers = false;
bool stream_magnitudes = false;
bool stream_spectrogram = false;
bool stream_chromagram = false;

bool debug_mode = false;
uint64_t chip_id = 0;
uint32_t chip_id_high = 0;
uint32_t chip_id_low  = 0;

uint32_t serial_iter = 0;

// ------------------------------------------------------------
// Spectrogram normalization (GDFT.h) -------------------------

float max_mags[NUM_ZONES] = { 0.000 };
float max_mags_followers[NUM_ZONES] = { 0.000 };
float mag_targets[NUM_FREQS] = { 0.000 };
float mag_followers[NUM_FREQS] = { 0.000 };
float mag_float_last[NUM_FREQS] = { 0.000 };
int32_t magnitudes[NUM_FREQS] = { 0 };
float magnitudes_normalized[NUM_FREQS] = { 0.000 };
float magnitudes_normalized_avg[NUM_FREQS] = { 0.000 };
float magnitudes_last[NUM_FREQS] = { 0.000 };
float magnitudes_final[NUM_FREQS] = { 0.000 };

// ------------------------------------------------------------
// Look-ahead smoothing (GDFT.h) ------------------------------

const uint8_t spectrogram_history_length = 3;
float spectrogram_history[spectrogram_history_length][64];
uint8_t spectrogram_history_index = 0;

// ------------------------------------------------------------
// Used for converting for storage in LittleFS (bridge_fs.h) --

union bytes_32 {
  uint32_t long_val;
  int32_t  long_val_signed;
  float    long_val_float;
  uint8_t  bytes[4];
};

// ------------------------------------------------------------
// Used for GDFT mode (lightshow_modes.h) ---------------------

uint8_t brightness_levels[NUM_FREQS] = { 0 };

// ------------------------------------------------------------
// Used for USB updates (system.h) ----------------------------

FirmwareMSC MSC_Update;
USBCDC USBSerial;
bool msc_update_started = false;

// DOTS
DOT dots[MAX_DOTS];

// Auto Color Shift
SQ15x16 hue_position = 0.0;
SQ15x16 hue_shift_speed = 0.0;
SQ15x16 hue_push_direction = -1.0;
SQ15x16 hue_destination = 0.0;
SQ15x16 hue_shifting_mix = -0.35;
SQ15x16 hue_shifting_mix_target = 1.0;

// VU Calculation
SQ15x16 audio_vu_level = 0.0;
SQ15x16 audio_vu_level_average = 0.0;
SQ15x16 audio_vu_level_last = 0.0;

// Knobs
KNOB knob_photons;
KNOB knob_chroma;
KNOB knob_mood;
uint8_t current_knob = K_NONE;

// Base Coat
SQ15x16 base_coat_width        = 0.0;
SQ15x16 base_coat_width_target = 1.0;

// Config File
char config_filename[24];

// WIP BELOW --------------------------------------------------

float MASTER_BRIGHTNESS = 0.0;
float last_sample = 0;