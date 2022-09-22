uint8_t LIGHTSHOW_MODE = DUET_MODE;
uint8_t MIRROR_ENABLED = false;

union bytes_32 {
  uint32_t long_val;
  int32_t  long_val_signed;
  uint8_t  bytes[4];
};

float fft_input  [BUFFER_SIZE];
float fft_output [BUFFER_SIZE]; 
ESP_fft FFT(BUFFER_SIZE, SAMPLE_RATE, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

float FFT_CEILING = 400000;
float BRIGHTNESS  = 1.000;
float SMOOTHING   = 0.570;

int32_t  i2s_samples_raw     [BUFFER_SIZE];
int32_t  i2s_samples         [6][BUFFER_SIZE];
int16_t  fft_integer         [BUFFER_SIZE];
int16_t  scaled_fft          [BUFFER_SIZE];
int16_t  final_fft           [FFT_HISTORY_LEN][BUFFER_SIZE];
float    processed_fft       [128];
float    fft_velocities      [128];
int16_t  fft_ambient_noise   [128];
uint8_t  ambient_noise_samples_collected = 0;
bool     collecting_ambient_noise = false;
int16_t  fft_history_index = 0;
size_t bytes_read;

CRGB leds[NUM_LEDS];
CRGB leds_out[STRIP_LED_COUNT];
CRGB leds_diffuse[NUM_LEDS];
CRGB leds_temp[NUM_LEDS]; // Used for effects
CRGB leds_last[NUM_LEDS]; // Used for keeping LED data between frames

float last_fft_frame[BUFFER_SIZE];
float hue = 100;
float hue_push = 0.0;
float hue_shift_amount = 0.40;

int32_t onset_pulse_history[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int32_t current_frame = 0;

uint8_t i2s_history_index = 0;

float FPS = 60.0;
float fft_vel_sum_history[360];
uint16_t fft_vel_sum_history_index = 0;
uint32_t tempo_animation_frame_cap = 90;
uint32_t tempo_animation_frame_cap_target = 90;
uint32_t tempo_animation_frame = 0;

int32_t max_vals[4] = {0};
int32_t max_vals_last[4] = {0};
int32_t max_vals_smoothed[4] = {0};

float multiplier_sum = 0.0;
float multipliers[4] = {0.01,0.01,0.01,0.01};
float multiplier_targets[4] = {1.00,1.00,1.00,1.00};
float multiplier_push = 0.015;
float multipliers_last[4] = {0.01,0.01,0.01,0.01};
float multipliers_smoothed[4] = {0.01,0.01,0.01,0.01};
uint16_t multiplier_centers[4]   = {16, 48, 80, 112};
uint16_t multiplier_bounds[4][2] = {{0,31}, {32, 63}, {64,95}, {96, 127}};

String current_section = "section_name";
uint32_t section_start = 0;
uint32_t section_end   = 0;

bool warn = false;
bool debug_mode = false;

int32_t DC_OFFSET =  0.0;

uint32_t last_setting_change = 0;
bool settings_updated = false;

struct button{
  uint8_t pin = 0;
  uint32_t last_down = 0;
  uint32_t last_up = 0;
  bool pressed = false;
};

button noise_button;
button mode_button;
