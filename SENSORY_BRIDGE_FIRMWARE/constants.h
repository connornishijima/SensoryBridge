// At 18,750 times a second, we capture a
// single sample of a buffer with length 256.
#define SAMPLE_RATE  18750
#define BUFFER_SIZE  256
#define I2S_PORT     I2S_NUM_0

// The last N runs of the FFT are kept for
// comparison and averaging
#define FFT_HISTORY_LEN 6

// What the average levels of the FFT multipliers should be to
// be considered in the "Sweet Spot"
#define SWEET_SPOT   0.45F

// How many average samples should we take 
// when calibrating ambient noise?
#define AMBIENT_NOISE_SAMPLES  128

// Internal resolution
#define NUM_LEDS  128

// Knob pins
#define PHOTONS_PIN  1
#define BOOST_PIN    2
#define MOOD_PIN     3

// Sweet Spot LED pins
#define SWEET_SPOT_LEFT_PIN    7
#define SWEET_SPOT_CENTER_PIN  8
#define SWEET_SPOT_RIGHT_PIN   9

// Button pins
#define NOISE_CAL_PIN  11
#define MODE_PIN       45

// Addressable LED pins
#define LED_DATA_PIN   36
#define LED_CLOCK_PIN  37

// I2S pins
#define I2S_BCLK_PIN   33
#define I2S_LRCLK_PIN  34
#define I2S_DOUT_PIN   35