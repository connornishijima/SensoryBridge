// ------------------------------------------------------------
// Configuration structure ------------------------------------

struct conf {
  // Synced values
  float PHOTONS;
  float CHROMA;
  float MOOD;
  float BASE_HUE;
  uint8_t LIGHTSHOW_MODE;
  uint8_t MIRROR_ENABLED;

  // Private values
  int32_t  DC_OFFSET; // (TODO)
  uint32_t WAVEFORM_NOISE_FLOOR;
  uint8_t  ESPNOW_CHANNEL;
  bool     IS_MAIN_UNIT;
};

conf CONFIG = { // Defaults of the CONFIG struct
  // Synced values
  1.00, // PHOTONS
  0.00, // CHROMA
  0.05, // MOOD
  0.00, // BASE_HUE
  LIGHT_MODE_GDFT, // LIGHTSHOW_MODE
  1,               // MIRROR_ENABLED (>= 3.0.0 defaults yes)

  // Private values
  0, // DC_OFFSET
  0, // WAVEFORM_NOISE_FLOOR
  3, // ESPNOW_CHANNEL
  false, // IS_MAIN_UNIT
};

// ------------------------------------------------------------
// Goertzel structure (generated in system.h) -----------------

struct freq {
  float target_freq;
  float block_size;
  float k;
  float w;
  float cosine;
  float sine;
  float coeff;
  float q0;
  float q1;
  float q2;
  float a_weighting_ratio;
};
freq frequencies[NUM_FREQS];

// ------------------------------------------------------------
// Hann window lookup table (generated in system.h) -----------

float window_lookup[256] = { 0 };

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

float note_spectrogram[NUM_FREQS] = {0};
float note_spectrogram_smooth[NUM_FREQS] = {0};
float note_spectrogram_long_term[NUM_FREQS] = {0};
float note_chromagram[12]  = {0};

// ------------------------------------------------------------
// Audio samples (i2s_audio.h) --------------------------------

uint16_t buf_len = SAMPLES_PER_CHUNK * 8;
char     *raw_i2s_buf = (char*)calloc(buf_len, sizeof(char));
int16_t  signed_audio[SAMPLES_PER_CHUNK] = {0};

float    sample_history[SAMPLE_HISTORY_LENGTH] = { 0.0 };
int16_t  sample_history_index = 0;
uint32_t i2s_read_errors = 0;

float current_sample = 0.0;
float averaged_sample = 0.0;

// ------------------------------------------------------------
// Noise calibration (noise_cal.h) ----------------------------

bool noise_complete = true;
float noise_samples[NUM_FREQS] = { 1 };
uint16_t noise_iterations = 0;

// ------------------------------------------------------------
// Display buffers (led_utilities.h) --------------------------

CRGB leds[128];
CRGB leds_temp[128];
CRGB leds_last[128];
CRGB leds_aux [128];
CRGB leds_fade[128];
CRGB leds_out[STRIP_LED_COUNT];

// ------------------------------------------------------------
// Benchmarking (system.h) ------------------------------------

Ticker cpu_usage;
volatile uint16_t function_id = 0;
volatile uint16_t function_hits[32] = {0};
float FPS = 0.0;

// ------------------------------------------------------------
// SensorySync P2P network (p2p.h) ----------------------------

bool main_override = true;
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

// ------------------------------------------------------------
// Settings tracking (system.h) -------------------------------

uint32_t last_setting_change = 0;
bool settings_updated = false;

// ------------------------------------------------------------
// Serial buffer (serial_menu.h) ------------------------------

char command_buf[64] = {0};
uint8_t command_buf_index = 0;

// ------------------------------------------------------------
// Spectrogram normalization (GDFT.h) -------------------------

float max_mags[NUM_ZONES] = { 0.000 };
float max_mags_followers[NUM_ZONES] = { 0.000 };
float mag_targets[NUM_FREQS] = { 0.000 };
float mag_followers[NUM_FREQS] = { 0.000 };
float mag_float_last[NUM_FREQS] = { 0.000 };
float magnitudes[NUM_FREQS] = { 0.000 };

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

// WIP BELOW --------------------------------------------------

float MASTER_BRIGHTHNESS = 0.0;
float last_sample = 0;

float retro_bulbs[NUM_FREQS]        = {0};
float retro_heat_targets[NUM_FREQS] = {0};

float scale_scores[24] = {0.0};
