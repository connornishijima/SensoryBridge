// Can return a value between two array indices with linear interpolation
SQ15x16 IRAM_ATTR interpolate(SQ15x16 index, SQ15x16* array, uint16_t array_size) {
  SQ15x16 index_f = index * (array_size - 1);
  uint16_t index_i = (uint16_t)index_f;
  SQ15x16 index_f_frac = index_f - index_i;

  SQ15x16 left_val  = array[index_i];
  SQ15x16 right_val = array[index_i + 1];

  if (index_i + 1 >= array_size) {
    right_val = left_val;
  }

  return (1 - index_f_frac) * left_val + index_f_frac * right_val;
}

// Convert and array of 4 bytes to a zero-padded hex string (9 chars to include null terminator)
void bytes_to_hex_string(const byte bytes[4], char hex_string[9]) {
  snprintf(hex_string, 9, "%02X%02X%02X%02X", bytes[0], bytes[1], bytes[2], bytes[3]);
}

void print_chip_id() {
  for (int i = 0; i < 17; i += 8) {
    chip_id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  char hex_string[9]; // ... Create a char array to store the hex string
  bytes_32 b; // ........... Create a bytes_32 struct to hold the bytes of chip_id_high

  b.long_val = chip_id; // ....................... Assign chip_id_low to the long_val member of the struct
  bytes_to_hex_string(b.bytes, hex_string); // ... Convert the bytes of chip_id_low to a zero-padded hex string
  USBSerial.print(hex_string); // ................ Print the hex string to the serial port

  USBSerial.println(); // Print a newline character
}

void blur_array(float* input, int length, int kernel_size) {
    int padding = kernel_size / 2;
    for (int i = 0; i < length; i++) {
        float sum = 0.0f;
        for (int j = -padding; j <= padding; j++) {
            int k = i + j;
            if (k < 0) k = 0;
            if (k >= length) k = length - 1;
            sum += input[k];
        }
        input[i] = sum / (float)kernel_size;
    }
}

float low_pass_filter(float new_data, float last_data, uint32_t sample_rate, float cutoff_freq) {
    float alpha = 1.0 - expf(-2.0 * PI * cutoff_freq / sample_rate);
    float output = (1.0 - alpha) * (last_data) + alpha * new_data;
    return output;
}

void low_pass_array(float* new_frame, float* last_frame, uint16_t length, uint32_t sample_rate, float cutoff_freq){
    for(uint16_t i = 0; i < length; i++){
        new_frame[i] = low_pass_filter(new_frame[i], last_frame[i], sample_rate, cutoff_freq);
    }
}

SQ15x16 low_pass_filter_fixed(SQ15x16 new_data, SQ15x16 last_data, uint32_t sample_rate, float cutoff_freq) {
    SQ15x16 alpha  = 1.0 - expf(-2.0 * PI * cutoff_freq / sample_rate);
    SQ15x16 output = SQ15x16(1.0 - alpha) * (last_data) + alpha * new_data;
    return output;
}

void low_pass_array_fixed(SQ15x16* new_frame, SQ15x16* last_frame, uint16_t length, uint32_t sample_rate, float cutoff_freq){
    for(uint16_t i = 0; i < length; i++){
        new_frame[i] = low_pass_filter_fixed(new_frame[i], last_frame[i], sample_rate, cutoff_freq);
    }
}

float random_float(){
    return esp_random() / (float)UINT32_MAX;
}

SQ15x16 mood_scale(SQ15x16 center, SQ15x16 range){
    SQ15x16 knob_value_bidirectional = (CONFIG.MOOD - 0.5) * SQ15x16(2.0); // 0.0 - 1.0 range transformed to -1.0 to +1.0
    SQ15x16 result = center + range*knob_value_bidirectional;

    return result;
}

SQ15x16 fabs_fixed(SQ15x16 input){
  if(input < SQ15x16(0.0)){
    input *= SQ15x16(-1.0);
  }

  return input;
}

SQ15x16 fmin_fixed(SQ15x16 a, SQ15x16 b) {
    return (a < b) ? a : b;
}

SQ15x16 fmax_fixed(SQ15x16 a, SQ15x16 b) {
    return (a > b) ? a : b;
}

SQ15x16 fmod_fixed(SQ15x16 dividend, SQ15x16 divisor) {
    SQ15x16 quotient = dividend / divisor;
    return dividend - (divisor * floorFixed(quotient));
}

float clip_float(float input){
  return min(1.0f, max(0.0f, input));
}