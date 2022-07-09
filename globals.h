float AUTORANGE_MIX = 0.0; // 0 = local only, 1 = focus only

uint8_t LIGHTSHOW_MODE = DUET_MODE;

union bytes_32 {
  uint32_t long_val;
  uint8_t  bytes[4];
};

float fft_input  [BUFFER_SIZE];
float fft_output [BUFFER_SIZE];
ESP_fft FFT(BUFFER_SIZE, SAMPLE_RATE, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

float FFT_CEILING = 400000;
float BRIGHTNESS  = 1.000;
float SMOOTHING   = 0.570;

int32_t  i2s_samples   [BUFFER_SIZE];
float    final_fft     [FFT_HISTORY_LEN][150];
float    processed_fft [128];
float    fft_velocities[128];
uint32_t fft_ambient_noise_sum[256];
uint8_t  ambient_noise_samples_collected = 0;
bool     collecting_ambient_noise = false;
int16_t  fft_history_index = 0;
size_t bytes_read;

CRGB leds[NUM_LEDS];
CRGB leds_temp[NUM_LEDS]; // Used for effects
CRGB leds_last[NUM_LEDS]; // Used for keeping LED data between frames

float last_fft_frame[128];
float hue = 100;
float hue_push = 0.0;
float hue_shift_amount = 0.40;

float multiplier_sum = 0.0;
float multipliers[6] = {0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
int16_t bin_boundaries[6][2] = {
  {0,  21},
  {22, 43},
  {44, 64},
  {65, 85},
  {86, 106},
  {107, 127},
};
float multipliers_history[MULTIPLIER_HISTORY_LENGTH][6] = {
  {0.01, 0.01, 0.01, 0.01, 0.01, 0.01}
};
int16_t multiplier_history_index = 0;
float multipliers_smoothed[6] = {0.01, 0.01, 0.01, 0.01, 0.01, 0.01};