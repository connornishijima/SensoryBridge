/*----------------------------------------
  Sensory Bridge GLOBAL VARIABLES
----------------------------------------*/

struct conf {
  // Synced values
  float PHOTONS;
  float CHROMA;
  float MOOD;
  float BASE_HUE;
  uint8_t LIGHTSHOW_MODE;
  uint8_t MIRROR_ENABLED;

  // Private values
  int32_t  DC_OFFSET;
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
  DUET_MODE, // LIGHTSHOW_MODE
  0,         // MIRROR_ENABLED

  // Private values
  0, // DC_OFFSET
  0, // WAVEFORM_NOISE_FLOOR
  3, // ESPNOW_CHANNEL
  false, // IS_MAIN_UNIT
};

float   MASTER_BRIGHTHNESS   = 0.0;
uint8_t sweet_spot_rough = 1; // center LED

float fft_input[FRAME_SIZE];
float fft_output[FRAME_SIZE];
float fft_temp[128];
float fft_output_last[128];
float fft_scaled[128] = {0};
float fft_processed[128] = {0};

uint8_t fft_output_history_index = 0;
const uint8_t fft_output_history_length = 3;
float fft_output_history[fft_output_history_length][128];

const uint8_t max_val_resolution = 4;
const uint8_t max_val_divisor = 128 / max_val_resolution;
float fft_max_vals[max_val_resolution] = {0};
float fft_max_vals_last[max_val_resolution] = {0};

float fft_max_move;
float fft_min_move;

float ambient_samples[128] = { 0 };
float ambient_samples_max = 0;
uint32_t ambient_frames_collected = 0;
uint32_t ambient_solve_time = 0;
uint32_t ambient_start_time = 0;
bool collecting_ambient_noise = false;

const uint8_t fft_history_length = 3;
float fft_history[fft_history_length][128];
uint8_t fft_history_index = 0;

int32_t i2s_samples_raw[BUFFER_SIZE] = { 0 };
int32_t i2s_samples[4][FRAME_SIZE] = { 0 };
uint8_t i2s_buffer_index = 0;
uint8_t i2s_history_index = 0;

const float window_lookup[256] = {
  0.00, 0.01, 0.02, 0.04, 0.05, 0.06, 0.07, 0.09, 0.10, 0.11, 0.12, 0.13, 0.15, 0.16, 0.17, 0.18, 0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.27, 0.28, 0.29, 0.30, 0.31, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48, 0.49, 0.50, 0.51, 0.52, 0.53, 0.55, 0.56, 0.57, 0.58, 0.59, 0.60, 0.61, 0.62, 0.62, 0.63, 0.64, 0.65, 0.66, 0.67, 0.68, 0.69, 0.70, 0.71, 0.72, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77, 0.77, 0.78, 0.79, 0.80, 0.80, 0.81, 0.82, 0.82, 0.83, 0.84, 0.84, 0.85, 0.86, 0.86, 0.87, 0.88, 0.88, 0.89, 0.89, 0.90, 0.90, 0.91, 0.91, 0.92, 0.92, 0.93, 0.93, 0.94, 0.94, 0.95, 0.95, 0.95, 0.96, 0.96, 0.96, 0.97, 0.97, 0.97, 0.98, 0.98, 0.98, 0.98, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.98, 0.98, 0.98, 0.98, 0.97, 0.97, 0.97, 0.96, 0.96, 0.96, 0.95, 0.95, 0.95, 0.94, 0.94, 0.93, 0.93, 0.92, 0.92, 0.91, 0.91, 0.90, 0.90, 0.89, 0.89, 0.88, 0.88, 0.87, 0.86, 0.86, 0.85, 0.84, 0.84, 0.83, 0.82, 0.82, 0.81, 0.80, 0.80, 0.79, 0.78, 0.77, 0.77, 0.76, 0.75, 0.74, 0.73, 0.72, 0.72, 0.71, 0.70, 0.69, 0.68, 0.67, 0.66, 0.65, 0.64, 0.63, 0.62, 0.62, 0.61, 0.60, 0.59, 0.58, 0.57, 0.56, 0.55, 0.53, 0.52, 0.51, 0.50, 0.49, 0.48, 0.47, 0.46, 0.45, 0.44, 0.43, 0.42, 0.41, 0.39, 0.38, 0.37, 0.36, 0.35, 0.34, 0.33, 0.31, 0.30, 0.29, 0.28, 0.27, 0.25, 0.24, 0.23, 0.22, 0.21, 0.20, 0.18, 0.17, 0.16, 0.15, 0.13, 0.12, 0.11, 0.10, 0.09, 0.07, 0.06, 0.05, 0.04, 0.02, 0.01
};

const float mic_balance[128] = {
  5.398954,5.373411,5.328011,5.257064,5.157744,5.030043,4.873964,4.689511,4.476677,4.235467,3.966278,3.704273,3.452300,3.213204,2.989825,2.785185,2.579161,2.372485,2.165887,1.967414,1.801886,1.659321,1.541368,1.456613,1.399520,1.347433,1.301594,1.262174,1.213456,1.149935,1.078443,1.017192,0.964711,0.911930,0.864598,0.827546,0.781027,0.718961,0.662197,0.619897,0.578382,0.536117,0.513951,0.511038,0.504644,0.503400,0.515157,0.533797,0.557706,0.600068,0.650986,0.704644,0.741537,0.742480,0.723983,0.686012,0.627253,0.559380,0.501839,0.455842,0.423358,0.403788,0.400367,0.411754,0.413965,0.408707,0.409391,0.403618,0.412986,0.417437,0.422213,0.408199,0.387205,0.342933,0.310858,0.278727,0.261609,0.252487,0.242893,0.224384,0.207354,0.186983,0.165864,0.152273,0.145800,0.138413,0.129207,0.120916,0.112267,0.106699,0.104228,0.102548,0.101354,0.098611,0.093962,0.094862,0.104526,0.114773,0.122439,0.129278,0.127812,0.124635,0.121433,0.122490,0.122749,0.121485,0.112052,0.109109,0.109751,0.111333,0.118965,0.138565,0.148987,0.159770,0.166720,0.179379,0.185571,0.200883,0.215715,0.232312,0.238292,0.242666,0.253266,0.261165,0.261102,0.249475,0.236936,0.213323,
};

CRGB leds[128];
CRGB leds_out[STRIP_LED_COUNT];
CRGB leds_last[128];
CRGB leds_temp[128];

float hue_scatter[128] = {0};
float fft_velocity = 0.0;
float hue_push = 0.0;

bool main_override = true;
uint32_t last_rx_time = 0;

union bytes_32 {
  uint32_t long_val;
  float    long_val_float;
  int32_t  long_val_signed;
  uint8_t  bytes[4];
};

struct button{
  uint8_t pin = 0;
  uint32_t last_down = 0;
  uint32_t last_up = 0;
  bool pressed = false;
};

button noise_button;
button mode_button;

uint32_t last_setting_change = 0;
bool settings_updated = false;

Ticker cpu_usage;
volatile uint16_t function_id = 0;
volatile uint16_t function_hits[32] = {0};

char command_buf[64] = {0};
uint8_t command_buf_index = 0;