#include <driver/i2s.h>

#define DATA_SAMPLE_RATE       24400
#define DATA_SAMPLES_PER_CHUNK 128
#define DATA_HISTORY_LENGTH    (SAMPLE_HISTORY_LENGTH/2)
#define BYTE_LENGTH            128
#define BIT_LENGTH             (BYTE_LENGTH*8)
#define WATCHDOG_TIMEOUT_US    1000000

uint8_t tone_freq_range = 3;

float readout_brightness = 0.25;

short sample_window_data[DATA_HISTORY_LENGTH] = { 0 };

uint8_t bit_data[BIT_LENGTH] = { 0 };
uint16_t total_bits = 0;

char character_data[BYTE_LENGTH] = { 0 };

enum step_names {
  WAITING,
  PULSE_MEASUREMENT,
  TONAL_MEASUREMENT,
  EMPTY_TONE_MEASUREMENT,
  DATA_TRANSMISSION,
  COMPLETE
};

freq data_frequencies[9];
const uint16_t data_frequencies_hz[9] = {
  697,  770,  852,  941,
  1209, 1336, 1477, 1633,

  440, // "EMPTY" frequency
};

uint32_t data_magnitudes[9]             = { 0 };
float    magnitude_multipliers[9]       = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
float    magnitude_multipliers_temp[9]  = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
uint16_t top_frequencies[9]             = { 0 };

float    magnitude_base_level = 100000;

bool     magic_tone_present = false;
uint32_t measured_pulse_duration = 0;
uint32_t measured_pulse_duration_sum = 0;
uint8_t  measured_pulse_count = 0;

bool tone_measurement_started = false;
uint32_t tone_measurement_started_time = -1;
uint8_t measured_tone_count = 0;

bool rx_begun = false;

uint32_t pulse_measurement_start_time = 0;
bool got_pulse = false;
uint32_t recent_pulse_time = 0;

uint8_t current_step = WAITING;

uint32_t last_watchdog_time = 0;

uint8_t recv_checksum = 0;
uint8_t calc_checksum = 0;
bool checksum_passed = false;

uint8_t current_nibble = 255;
uint32_t complete_time = 0;

float led_expansion = 0.0;

uint8_t convert_bit_data_to_string() {
  uint16_t bit_index = 0;
  uint16_t byte_index = 0;
  uint8_t checksum = 0;
  for (uint16_t i = 0; i < total_bits; i++) {
    if (i <= total_bits - 8) {
      bitWrite(character_data[byte_index], 7 - bit_index, bit_data[i]);
    }
    else {
      bitWrite(checksum, 7 - bit_index, bit_data[i]);
    }

    bit_index++;
    if (bit_index >= 8) {
      bit_index = 0;
      byte_index++;
    }
  }

  return checksum;
}

void reset_vars() {
  for (uint16_t i = 0; i < BIT_LENGTH; i++) {
    bit_data[i] = 0;
  }
  for (uint16_t i = 0; i < BYTE_LENGTH; i++) {
    character_data[i] = 0;
  }
  total_bits = 0;

  magic_tone_present = false;
  pulse_measurement_start_time = 0;
  measured_pulse_duration = 0;
  measured_pulse_duration_sum = 0;
  measured_pulse_count = 0;

  tone_measurement_started = false;
  tone_measurement_started_time = 0;
  measured_tone_count = 0;

  current_nibble = 255;
  checksum_passed = false;
  complete_time = 0;

  got_pulse = false;
  for (uint8_t i = 0; i < 9; i++) {
    data_magnitudes[i]            = 0;
    magnitude_multipliers[i]      = 1.0;
    magnitude_multipliers_temp[i] = 1.0;
    top_frequencies[i]            = 0;
  }

  last_watchdog_time = 0;

  led_expansion = 0.0;
  rx_begun = false;

  current_step = WAITING;
}

void watchdog_notify(uint32_t t_now_us) {
  last_watchdog_time = t_now_us;
}

void watch_timeout(uint32_t t_now_us) {
  if (t_now_us - last_watchdog_time > WATCHDOG_TIMEOUT_US) {
    USBSerial.println("!-- TIMEOUT!");

    current_step = COMPLETE;
    checksum_passed = false;
    complete_time = t_now_us;
  }
}

void init_i2s_for_data() {
  const i2s_config_t i2s_config = { // This version is different from i2s_audio.h, with values that best suit this use-case
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = DATA_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .dma_buf_count = 1024 / (DATA_SAMPLES_PER_CHUNK * 2),
    .dma_buf_len = (DATA_SAMPLES_PER_CHUNK * 2)
  };

  const i2s_pin_config_t pin_config = { // These too
    .bck_io_num   = I2S_BCLK_PIN,
    .ws_io_num    = I2S_LRCLK_PIN,
    .data_out_num = -1,  // not used (only for outputs)
    .data_in_num  = I2S_DIN_PIN
  };

  // Init I2S Driver
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  USBSerial.print("INIT I2S: ");
  USBSerial.println(result == ESP_OK ? PASS : FAIL);

  if (result != ESP_OK) {
    pinMode(8, OUTPUT);
    while (true) {
      USBSerial.println("INIT FAIL");
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
      delay(100);
    }
  }

  // ESP32-S2 changes to help SPH0645 mic
  REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
  REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

  // Set I2S pins
  result = i2s_set_pin(I2S_PORT, &pin_config);
  USBSerial.print("I2S SET PINS: ");
  USBSerial.println(result == ESP_OK ? PASS : FAIL);
}

void init_data_frequencies() {
  for (uint8_t i = 0; i < 9; i++) {
    data_frequencies[i].target_freq = data_frequencies_hz[i];
    data_frequencies[i].block_size  = 300;
    data_frequencies[i].block_size_recip = 1.0 / float(data_frequencies[i].block_size);

    float w = 2.0 * PI * ((float)data_frequencies[i].target_freq / DATA_SAMPLE_RATE);
    float coeff = 2.0 * cos(w);
    data_frequencies[i].coeff_q14 = (1 << 14) * coeff;
  }
}

void acquire_data_chunk() {
  size_t bytes_read = 0;
  i2s_read(I2S_PORT, i2s_samples_raw, DATA_SAMPLES_PER_CHUNK * sizeof(int32_t), &bytes_read, portMAX_DELAY);

  // Scale I2S samples and store into history
  for (uint16_t i = 0; i < DATA_SAMPLES_PER_CHUNK; i++) {
    int32_t sample = (i2s_samples_raw[i] * 0.000512) + 56000 - 5120;

    sample = sample >> 2; // Helps prevent overflow in fixed-point math coming up

    if (sample > 32767) { // clipping
      sample = 32767;
    } else if (sample < -32767) {
      sample = -32767;
    }

    waveform[i] = sample - CONFIG.DC_OFFSET;
  }

  for (int i = 0; i < DATA_HISTORY_LENGTH - DATA_SAMPLES_PER_CHUNK; i++) {
    sample_window_data[i] = sample_window_data[i + DATA_SAMPLES_PER_CHUNK];
  }
  for (int i = DATA_HISTORY_LENGTH - DATA_SAMPLES_PER_CHUNK; i < DATA_HISTORY_LENGTH; i++) {
    sample_window_data[i] = waveform[i - (DATA_HISTORY_LENGTH - DATA_SAMPLES_PER_CHUNK)];
  }
}

void fast_gdft() {
  // Run GDFT (Goertzel - based Discrete Fourier Transform) with 9 frequencies
  for (uint16_t i = 0; i < 9; i++) {
    int32_t q0, q1, q2;
    int64_t mult;

    q1 = 0;
    q2 = 0;

    for (uint16_t n = 0; n < data_frequencies[i].block_size; n++) { // Run Goertzel for "block_size" iterations
      int32_t sample = sample_window_data[DATA_HISTORY_LENGTH - 1 - n];
      mult = (int64_t)data_frequencies[i].coeff_q14 * (int64_t)q1;
      q0 = (sample >> 6) + (mult >> 14) - q2;
      q2 = q1;
      q1 = q0;
    }

    mult = (int64_t)data_frequencies[i].coeff_q14 * (int64_t)q1;
    data_magnitudes[i] = q2 * q2 + q1 * q1 - ((int32_t)(mult >> 14)) * q2; // Calculate raw magnitudes

    // Normalize output
    data_magnitudes[i] *= float(data_frequencies[i].block_size_recip);

    if (data_magnitudes[i] < 0.0) { // Prevent negative values
      data_magnitudes[i] = 0.0;
    }

    data_magnitudes[i] *= magnitude_multipliers[i];
  }
}

unsigned char checksum(char *data, uint8_t len) {
  unsigned char result = 0;
  for (uint8_t i = 0; i < len; i++) {
    result ^= data[i];
  }
  return result;
}

bool top_frequency(uint16_t freq, uint8_t range) {
  bool found = false;
  for (uint8_t i = 0; i < range; i++) {
    if (top_frequencies[i] == freq) {
      found = true;
    }
  }

  return found;
}

void sort_top_frequencies() {
  bool freqs_found[9]  = { false };
  uint8_t current_index = 0;

  for (uint8_t i = 0; i < 9; i++) {
    top_frequencies[i] = 0;
  }

  while (current_index < 9) {
    uint32_t max_val = 0;
    uint8_t max_index = 255;
    for (uint8_t i = 0; i < 9; i++) {
      if (freqs_found[i] == false) {
        if ((data_magnitudes[i]) > max_val || (data_magnitudes[i]) == 0) {
          max_val = (data_magnitudes[i]);
          max_index = i;
        }
      }
    }

    if (max_index != 255) {
      top_frequencies[current_index] = data_frequencies_hz[max_index];
      freqs_found[max_index] = true;
      current_index++;
    }
  }
}

void append_two_bits(uint8_t bit_0, uint8_t bit_1) {
  bit_data[total_bits + 0] = bit_0;
  bit_data[total_bits + 1] = bit_1;

  total_bits += 2;
}

void calculate_tone_multiplier(uint8_t index) {
  magnitude_multipliers_temp[index] = magnitude_base_level / float(data_magnitudes[index]);
}

void parse_data() {
  static uint32_t iter = 0;
  uint32_t t_now_us = micros();
  iter++;

  /*
    for (uint8_t i = 0; i < 4; i++) {
    USBSerial.print(top_frequencies[i]);
    USBSerial.print("\t");
    }
    USBSerial.println();
  */

  if (current_step == WAITING) {
    if (top_frequency(697, 4) && top_frequency(1209, 4) && top_frequency(941, 4) && top_frequency(1633, 4)) {
      if (magic_tone_present == false) {
        USBSerial.println("MAGIC TONE!");
        magic_tone_present = true;
      }
    }
    else if (top_frequency(440, 2)) { // Hearing "empty" air
      if (magic_tone_present == true) {
        magic_tone_present = false;

        //USBSerial.println("MEASURING PULSE LENGTH...");
        current_step = PULSE_MEASUREMENT;
        led_expansion = 0.0;
        watchdog_notify(t_now_us);
      }
    }
  }

  else if (current_step == PULSE_MEASUREMENT) {
    if (top_frequency(697, 4) && top_frequency(1209, 4) && top_frequency(941, 4) && top_frequency(1633, 4)) {
      if (magic_tone_present == false) {
        magic_tone_present = true;
        pulse_measurement_start_time = t_now_us;
      }
    }
    else {
      if (magic_tone_present == true) {
        if (top_frequency(440, 2)) { // Hearing "empty" air
          magic_tone_present = false;

          measured_pulse_duration = t_now_us - pulse_measurement_start_time;

          measured_pulse_duration_sum += measured_pulse_duration;
          measured_pulse_count++;
          watchdog_notify(t_now_us);

          if (measured_pulse_count == 4) {
            measured_pulse_duration = measured_pulse_duration_sum / float(measured_pulse_count);
            measured_pulse_duration /= 4.0;
            measured_pulse_count = 0;
            measured_pulse_duration_sum = 0;

            //USBSerial.print("PULSE LENGTH: ");
            //USBSerial.print(measured_pulse_duration);
            //USBSerial.println("us");

            //USBSerial.println("MEASURING TONAL RESPONSE...");
            current_step = TONAL_MEASUREMENT;
            watchdog_notify(t_now_us);
          }
        }
      }
    }

    watch_timeout(t_now_us);
  }

  else if (current_step == TONAL_MEASUREMENT) {
    if (top_frequency(440, 2)) { // Hearing "empty" air
      if (got_pulse == true) {
        got_pulse = false;
      }
      if (tone_measurement_started == true) {
        tone_measurement_started = false;
      }

      bool solved = true;
      for (uint8_t i = 0; i < 8; i++) {
        if (magnitude_multipliers_temp[i] == 1.0000) {
          solved = false;
        }
      }

      if (solved == true) {
        tone_measurement_started_time = -1; // max

        //USBSerial.println("MEASURING EMPTY TONE...");
        current_step = EMPTY_TONE_MEASUREMENT;
        watchdog_notify(t_now_us);
        delay(10);
      }
    }
    else { // hearing tone
      if (tone_measurement_started == false) {
        tone_measurement_started = true;
        tone_measurement_started_time = t_now_us;
      }
      if (got_pulse == false) {
        bool tones_valid = false;
        bool time_to_measure = false;
        if (t_now_us >= tone_measurement_started_time + (measured_pulse_duration)) {
          time_to_measure = true;
        }

        if (time_to_measure == true) {
          if (top_frequency(697, 3) && top_frequency(1209, 3)) {
            tones_valid = true;
            //USBSerial.println("GOT PAIR 1");
            calculate_tone_multiplier(0);
            calculate_tone_multiplier(4);
            measured_tone_count++;
            watchdog_notify(t_now_us);
            tone_measurement_started_time = -1; // max
          }
          else if (top_frequency(770, 3) && top_frequency(1336, 3)) {
            tones_valid = true;
            //USBSerial.println("GOT PAIR 2");
            calculate_tone_multiplier(1);
            calculate_tone_multiplier(5);
            measured_tone_count++;
            watchdog_notify(t_now_us);
            tone_measurement_started_time = -1; // max
          }

          else if (top_frequency(852, 3) && top_frequency(1477, 3)) {
            tones_valid = true;
            //USBSerial.println("GOT PAIR 3");
            calculate_tone_multiplier(2);
            calculate_tone_multiplier(6);
            measured_tone_count++;
            watchdog_notify(t_now_us);
            tone_measurement_started_time = -1; // max
          }

          else if (top_frequency(941, 3) && top_frequency(1633, 3)) {
            tones_valid = true;
            //USBSerial.println("GOT PAIR 4");
            calculate_tone_multiplier(3);
            calculate_tone_multiplier(7);
            measured_tone_count++;
            watchdog_notify(t_now_us);
            tone_measurement_started_time = -1; // max
          }

          if (tones_valid) {
            // print tone count
          }
        }

        got_pulse = tones_valid;
      }
    }

    watch_timeout(t_now_us);
  }

  else if (current_step == EMPTY_TONE_MEASUREMENT) {
    if (top_frequency(440, 2)) { // Hearing "empty" air
      // Get "empty tone" measurement
      magnitude_multipliers_temp[8] = magnitude_base_level / float(data_magnitudes[8]);
      for (uint8_t i = 0; i < 9; i++) {
        magnitude_multipliers[i] = magnitude_multipliers_temp[i];
      }

      USBSerial.print("BEGIN RX...");
      current_step = DATA_TRANSMISSION;
      rx_begun = false;
      watchdog_notify(t_now_us);
    }

    watch_timeout(t_now_us);
  }

  else if (current_step == DATA_TRANSMISSION) {
    if (top_frequency(440, 2)) { // Hearing "empty" air
      if (got_pulse == true) {
        got_pulse = false;
      }

      if (total_bits >= 8) {
        if (t_now_us - recent_pulse_time >= (measured_pulse_duration * 4)) { // if >= pulse_duration*4 since last pulse
          USBSerial.println("DONE!");
          recv_checksum = convert_bit_data_to_string();
          calc_checksum = checksum(character_data, strlen(character_data));

          if (recv_checksum == calc_checksum) {
            checksum_passed = true;
          }

          current_step = COMPLETE;
          complete_time = t_now_us;
        }
      }
    }
    else { // Hearing data
      if (rx_begun == false) {
        rx_begun = true;
        led_expansion = 0.0;
      }

      if (got_pulse == false) {
        bool tones_valid = false;
        if (top_frequency(697, tone_freq_range) && top_frequency(1209, tone_freq_range)) {
          tones_valid = true;
          current_nibble = 0;
          append_two_bits(0, 0);
        }
        else if (top_frequency(770, tone_freq_range) && top_frequency(1336, tone_freq_range)) {
          tones_valid = true;
          current_nibble = 1;
          append_two_bits(0, 1);
        }
        else if (top_frequency(852, tone_freq_range) && top_frequency(1477, tone_freq_range)) {
          tones_valid = true;
          current_nibble = 2;
          append_two_bits(1, 0);
        }
        else if (top_frequency(941, tone_freq_range) && top_frequency(1633, tone_freq_range)) {
          tones_valid = true;
          current_nibble = 3;
          append_two_bits(1, 1);
        }

        if (tones_valid) {
          recent_pulse_time = t_now_us;
        }

        got_pulse = tones_valid;
      }
    }
  }

  else if (current_step == COMPLETE) {
    if (t_now_us - complete_time >= 500000) {
      reset_vars();
      current_step = WAITING;
    }
    else {
      USBSerial.print("MESSAGE: |");
      USBSerial.print(character_data);
      USBSerial.println("|");
    }
  }
}

void pass_animation() {
  uint32_t t_start = millis();
  float width = 0.0;
  float iter = 0.0;
  float wiggle_strength = 1.0;
  while (millis() < t_start + 1000) {
    iter += 0.15;
    
    if(width < 1.0){
      float delta = 1.0-width;
      width += delta*0.05;
    }
    
    clear_all_led_buffers();
    draw_line(leds, CRGB(0,255*width*width,0), 64, 64+(16*width)+(12*(sin(iter)+1.0)*(1.0-width)));
    mirror_image_downwards();
    show_leds();
  }

  while (width > 0.001) {
    width *= 0.95;
    clear_all_led_buffers();
    draw_line(leds, CRGB(0,255*width*width,0), 64, 64+(16*width)+(12*(sin(iter)+1.0)*(1.0-width)));
    mirror_image_downwards();
    show_leds();
  }
}

void fail_animation() {
  uint32_t t_start = millis();
  float width = 0.0;
  float midpoint = (NATIVE_RESOLUTION/2.0)-0.5;
  
  while (fabs(width - 1.0) > 0.01){
    if(width < 1.0){
      float delta = 1.0-width;
      width += delta*0.2;
    }

    float line_size = 16*width;
    
    clear_all_led_buffers();
    draw_line(leds, CRGB(255*width*width,0,0), midpoint+line_size, midpoint-line_size);
    show_leds();
  }
  width = 1.0;

  float iter = 0.0;
  while(iter < 20.0){
    iter += 0.2;
    float line_size = 16;

    float line_offset = sin(iter)*5.0;
    
    clear_all_led_buffers();
    draw_line(leds, CRGB(255*width*width,0,0), (midpoint+line_size) + line_offset, (midpoint-line_size) + line_offset);
    show_leds();
  }
  
  while (width > 0.005) {
    width *= 0.95;

    float line_size = 16*width;
    
    clear_all_led_buffers();
    draw_line(leds, CRGB(255*width*width,0,0), midpoint+line_size, midpoint-line_size);
    mirror_image_downwards();
    show_leds();
  }
  clear_all_led_buffers();
  show_leds();
}

void run_leds() {
  fadeToBlackBy(leds, NATIVE_RESOLUTION, 32);

  if (led_expansion < 1.0) {
    float delta = 1.0 - led_expansion;

    led_expansion += delta * 0.025;
  }

  if (current_step == WAITING) {
    static float iter = 0;
    iter += 0.03;
    float led_pos = sin(iter);
    led_pos += 1.0;
    led_pos /= 2.0;
    led_pos *= ((NATIVE_RESOLUTION / 2) - 1);

    leds[64 + uint16_t(led_pos)] = CRGB(0, 92, 255);
  }

  /*
    WAITING,
    PULSE_MEASUREMENT,
    TONAL_MEASUREMENT,
    EMPTY_TONE_MEASUREMENT,
    DATA_TRANSMISSION,
    COMPLETE
  */

  else if (current_step == PULSE_MEASUREMENT) {
    if (magic_tone_present == true) {
      for (uint8_t i = 0; i < 4; i++) {
        leds[64 + 6 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness, 0);
        leds[64 + 7 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness, 0);
        leds[64 + 8 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness, 0);
        leds[64 + 9 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness, 0);
      }
    }
    else {
      for (uint8_t i = 0; i < 4; i++) {
        leds[64 + 6 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness*0.25, 0);
        leds[64 + 7 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness*0.25, 0);
        leds[64 + 8 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness*0.25, 0);
        leds[64 + 9 + uint16_t(i * 16 * led_expansion)] = CRGB(0, 255*readout_brightness*0.25, 0);
      }
    }
  }

  else if (current_step == TONAL_MEASUREMENT) {
    for (uint8_t i = 0; i < measured_tone_count; i++) {
      leds[64 + 6 + uint16_t(i * 16 * led_expansion)] = CRGB(255*readout_brightness, 32*readout_brightness, 0);
      leds[64 + 7 + uint16_t(i * 16 * led_expansion)] = CRGB(255*readout_brightness, 32*readout_brightness, 0);
      leds[64 + 8 + uint16_t(i * 16 * led_expansion)] = CRGB(255*readout_brightness, 32*readout_brightness, 0);
      leds[64 + 9 + uint16_t(i * 16 * led_expansion)] = CRGB(255*readout_brightness, 32*readout_brightness, 0);
    }
  }

  else if (current_step == DATA_TRANSMISSION) {
    for(uint8_t i = 0; i < 4; i++){
      leds[64 + 6 + uint16_t(16 * i * led_expansion)] = CRGB(0, 92*readout_brightness*0.25, 255*readout_brightness*0.25);
      leds[64 + 7 + uint16_t(16 * i * led_expansion)] = CRGB(0, 92*readout_brightness*0.25, 255*readout_brightness*0.25);
      leds[64 + 8 + uint16_t(16 * i * led_expansion)] = CRGB(0, 92*readout_brightness*0.25, 255*readout_brightness*0.25);
      leds[64 + 9 + uint16_t(16 * i * led_expansion)] = CRGB(0, 92*readout_brightness*0.25, 255*readout_brightness*0.25);
    }
    
    leds[64 + 6 + uint16_t(16 * current_nibble * led_expansion)] = CRGB(0, 92*readout_brightness*2.0, 255*readout_brightness*2.0);
    leds[64 + 7 + uint16_t(16 * current_nibble * led_expansion)] = CRGB(0, 92*readout_brightness*2.0, 255*readout_brightness*2.0);
    leds[64 + 8 + uint16_t(16 * current_nibble * led_expansion)] = CRGB(0, 92*readout_brightness*2.0, 255*readout_brightness*2.0);
    leds[64 + 9 + uint16_t(16 * current_nibble * led_expansion)] = CRGB(0, 92*readout_brightness*2.0, 255*readout_brightness*2.0);
    current_nibble = 255;
  }

  else if (current_step == COMPLETE) {
    if (checksum_passed == true) {
      pass_animation();
    }
    else {
      fail_animation();
    }
  }

  mirror_image_downwards();
  show_leds();
}

void enable_audio_transfer_mode() {
  init_i2s_for_data();
  init_data_frequencies();

  while (true) {
    acquire_data_chunk();
    fast_gdft();
    sort_top_frequencies();
    parse_data();
    run_leds();

    yield();
  }
}
