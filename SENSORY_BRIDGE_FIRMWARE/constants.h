// AUDIO #######################################################

#define SERIAL_BAUD 230400
#define DEFAULT_SAMPLE_RATE 12800
#define SAMPLE_HISTORY_LENGTH 4096

// Don't change this unless you're willing to do a lot of other work on the code :/
#define NATIVE_RESOLUTION 128
#define NUM_FREQS 64
#define NUM_ZONES 2

#define I2S_PORT I2S_NUM_0

#define SPECTRAL_HISTORY_LENGTH 5

#define MAX_DOTS 128

enum reserved_dots {
  GRAPH_NEEDLE,
  GRAPH_DOT_1,
  GRAPH_DOT_2,
  GRAPH_DOT_3,
  GRAPH_DOT_4,
  GRAPH_DOT_5,
  RIPPLE_LEFT,
  RIPPLE_RIGHT,

  RESERVED_DOTS
};

enum knob_names {
  K_NONE,
  K_PHOTONS,
  K_CHROMA,
  K_MOOD
};

struct CRGB16 {  // Unsigned Q8.8 Fixed-point color channels
  SQ15x16 r;
  SQ15x16 g;
  SQ15x16 b;
};

struct DOT {
  SQ15x16 position;
  SQ15x16 last_position;
};

struct KNOB {
  SQ15x16  value;
  SQ15x16  last_value;
  SQ15x16  change_rate;
  uint32_t last_change;
};

const float notes[] = {
  55.00000, 58.27047, 61.73541, 65.40639, 69.29566, 73.41619, 77.78175, 82.40689, 87.30706, 92.49861, 97.99886, 103.8262,
  110.0000, 116.5409, 123.4708, 130.8128, 138.5913, 146.8324, 155.5635, 164.8138, 174.6141, 184.9972, 195.9977, 207.6523,
  220.0000, 233.0819, 246.9417, 261.6256, 277.1826, 293.6648, 311.1270, 329.6276, 349.2282, 369.9944, 391.9954, 415.3047,
  440.0000, 466.1638, 493.8833, 523.2511, 554.3653, 587.3295, 622.2540, 659.2551, 698.4565, 739.9888, 783.9909, 830.6094,
  880.0000, 932.3275, 987.7666, 1046.502, 1108.731, 1174.659, 1244.508, 1318.510, 1396.913, 1479.978, 1567.982, 1661.219,
  1760.000, 1864.655, 1975.533, 2093.005, 2217.461, 2349.318, 2489.016, 2637.020, 2793.825, 2959.956, 3135.964, 3322.437,
  3520.000, 3729.310, 3951.065, 4186.009, 4434.922, 4698.636, 4978.032, 5274.041, 5587.652, 5919.911, 6271.927, 6644.875,
  7040.000, 7458.620, 7902.130, 8372.018, 8869.844, 9397.272, 9956.064, 10548.08, 11175.30, 11839.82, 12543.85, 13289.75
};

// GPIO PINS #######################################################

#if (defined(LUMOSSTICK) || defined(LUMOSRING))  // LUMOS STICK or LOMOS RING PINS
#define I2S_BCLK_PIN    13 // labeled SCK on INMP441
#define I2S_LRCLK_PIN   14 // labeled WS  on INMP441
#define I2S_DIN_PIN     10 // labeled SD  on INMP441
// NOTE: L/R on the INMP441 needs to be HIGH. Solder a wire from VDD to L/R on the microphone

#ifdef LUMOSSTICK
#define LED_DATA_PIN    33    // this is just the first pin of 7 on the LumosStick
#define LUMOS_LEN       40		// each segment has 40 LEDs
#define LUMOS_SEGMENTS  7     // there are 7 segments, each has its own control pin
#define TOTAL_LED_COUNT (LUMOS_SEGMENTS * LUMOS_LEN)
#else
#define LED_DATA_PIN    16    // this is just the ring pin on the LumosRing (the block is pin 17)
#define TOTAL_LED_COUNT 240   // the LumosRing is made of of 60 bands of 4 LEDs each, all in series
#endif

#define NOISE_CAL_PIN   11    // SW1
#define MODE_PIN        12    // SW2

#define NO_SWEET_SPOT         // disable sweet spot code
#define LUMOS_DEVICE          // simplify remaining conditional code

#else                                         // SENSORY BRIDGE PINs
#define PHOTONS_PIN 1
#define CHROMA_PIN 2
#define MOOD_PIN 3

#define I2S_BCLK_PIN 33
#define I2S_LRCLK_PIN 34
#define I2S_DIN_PIN 35

#define LED_DATA_PIN 36
#define LED_CLOCK_PIN 37
#define TOTAL_LED_COUNT 128

#define RNG_SEED_PIN 10 // no longer used

#define NOISE_CAL_PIN 11
#define MODE_PIN 45

#define SWEET_SPOT_LEFT_PIN 7
#define SWEET_SPOT_CENTER_PIN 8
#define SWEET_SPOT_RIGHT_PIN 9

#endif

// OTHER #######################################################

const SQ15x16 dither_table[4] = {
  0.25,
  0.50,
  0.75,
  1.00
  /*
  0.166666,
  0.333333,
  0.500000,
  0.666666,
  0.833333,
  1.000000
  */
};

SQ15x16 note_colors[12] = {
  0.0000,
  0.0833,
  0.1666,
  0.2499,
  0.3333,
  0.4166,
  0.4999,
  0.5833,
  0.6666,
  0.7499,
  0.8333,
  0.9166
};

const SQ15x16 hue_lookup[64][3] = {
  { 1.0000, 0.0000, 0.0000 },
  { 0.9608, 0.0392, 0.0000 },
  { 0.9176, 0.0824, 0.0000 },
  { 0.8745, 0.1255, 0.0000 },
  { 0.8314, 0.1686, 0.0000 },
  { 0.7922, 0.2078, 0.0000 },
  { 0.7490, 0.2510, 0.0000 },
  { 0.7059, 0.2941, 0.0000 },
  { 0.6706, 0.3333, 0.0000 },
  { 0.6706, 0.3725, 0.0000 },
  { 0.6706, 0.4157, 0.0000 },
  { 0.6706, 0.4588, 0.0000 },
  { 0.6706, 0.5020, 0.0000 },
  { 0.6706, 0.5412, 0.0000 },
  { 0.6706, 0.5843, 0.0000 },
  { 0.6706, 0.6275, 0.0000 },
  { 0.6706, 0.6667, 0.0000 },
  { 0.5882, 0.7059, 0.0000 },
  { 0.5059, 0.7490, 0.0000 },
  { 0.4196, 0.7922, 0.0000 },
  { 0.3373, 0.8353, 0.0000 },
  { 0.2549, 0.8745, 0.0000 },
  { 0.1686, 0.9176, 0.0000 },
  { 0.0863, 0.9608, 0.0000 },
  { 0.0000, 1.0000, 0.0000 },
  { 0.0000, 0.9608, 0.0392 },
  { 0.0000, 0.9176, 0.0824 },
  { 0.0000, 0.8745, 0.1255 },
  { 0.0000, 0.8314, 0.1686 },
  { 0.0000, 0.7922, 0.2078 },
  { 0.0000, 0.7490, 0.2510 },
  { 0.0000, 0.7059, 0.2941 },
  { 0.0000, 0.6706, 0.3333 },
  { 0.0000, 0.5882, 0.4157 },
  { 0.0000, 0.5059, 0.4980 },
  { 0.0000, 0.4196, 0.5843 },
  { 0.0000, 0.3373, 0.6667 },
  { 0.0000, 0.2549, 0.7490 },
  { 0.0000, 0.1686, 0.8353 },
  { 0.0000, 0.0863, 0.9176 },
  { 0.0000, 0.0000, 1.0000 },
  { 0.0392, 0.0000, 0.9608 },
  { 0.0824, 0.0000, 0.9176 },
  { 0.1255, 0.0000, 0.8745 },
  { 0.1686, 0.0000, 0.8314 },
  { 0.2078, 0.0000, 0.7922 },
  { 0.2510, 0.0000, 0.7490 },
  { 0.2941, 0.0000, 0.7059 },
  { 0.3333, 0.0000, 0.6706 },
  { 0.3725, 0.0000, 0.6314 },
  { 0.4157, 0.0000, 0.5882 },
  { 0.4588, 0.0000, 0.5451 },
  { 0.5020, 0.0000, 0.5020 },
  { 0.5412, 0.0000, 0.4627 },
  { 0.5843, 0.0000, 0.4196 },
  { 0.6275, 0.0000, 0.3765 },
  { 0.6667, 0.0000, 0.3333 },
  { 0.7059, 0.0000, 0.2941 },
  { 0.7490, 0.0000, 0.2510 },
  { 0.7922, 0.0000, 0.2078 },
  { 0.8353, 0.0000, 0.1647 },
  { 0.8745, 0.0000, 0.1255 },
  { 0.9176, 0.0000, 0.0824 },
  { 0.9608, 0.0000, 0.0392 },
};

#define SWEET_SPOT_LEFT_CHANNEL 0
#define SWEET_SPOT_CENTER_CHANNEL 1
#define SWEET_SPOT_RIGHT_CHANNEL 2

#define TWOPI 6.28318530
#define FOURPI 12.56637061
#define SIXPI 18.84955593

enum led_types {
  LED_NEOPIXEL,
  LED_DOTSTAR
};

CRGB16 incandescent_lookup = { 1.0000, 0.4453, 0.1562 };