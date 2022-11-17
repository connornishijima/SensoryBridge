/*----------------------------------------
  Sensory Bridge GLOBAL CONSTANTS
----------------------------------------*/

#define BUFFER_SIZE 256
#define BUFFER_COUNT 1

#define FRAME_SIZE (BUFFER_SIZE * BUFFER_COUNT)
#define FFT_SIZE FRAME_SIZE / 2

#define SAMPLE_RATE 18750
#define I2S_PORT I2S_NUM_0

// I2S pins
#define I2S_BCLK_PIN  33
#define I2S_LRCLK_PIN 34
#define I2S_DOUT_PIN  35

#define LED_DATA_PIN 36

#define SWEET_SPOT_LEFT_PIN    7
#define SWEET_SPOT_CENTER_PIN  8
#define SWEET_SPOT_RIGHT_PIN   9

#define NOISE_CAL_PIN  11
#define MODE_PIN       45

#define PHOTONS_PIN 1
#define CHROMA_PIN  2
#define MOOD_PIN    3