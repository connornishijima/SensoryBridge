#define SAMPLE_RATE  18750
#define BUFFER_SIZE  256
#define I2S_PORT     I2S_NUM_0

#define FFT_HISTORY_LEN            6
#define AMBIENT_NOISE_SAMPLES      128

#define SWEET_SPOT   0.25F

#define PHOTONS_PIN  1
#define BOOST_PIN    2
#define MOOD_PIN     3

#define SWEET_SPOT_LEFT_PIN    7
#define SWEET_SPOT_CENTER_PIN  8
#define SWEET_SPOT_RIGHT_PIN   9

#define NOISE_CAL_PIN  11
#define MODE_PIN       45

#define NUM_LEDS       128 + LED_OFFSET
#define LED_DATA_PIN   36
#define LED_CLOCK_PIN  37

#define I2S_BCLK_PIN   33
#define I2S_LRCLK_PIN  34
#define I2S_DOUT_PIN   35
