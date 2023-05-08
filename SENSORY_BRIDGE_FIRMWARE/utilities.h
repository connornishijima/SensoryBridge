// Can return a value between two array indices with linear interpolation
float IRAM_ATTR interpolate(float index, float* array, uint16_t array_size) {
  float index_f = index * (array_size - 1);
  uint16_t index_i = (uint16_t)index_f;
  float index_f_frac = index_f - index_i;

  float left_val  = array[index_i];
  float right_val = array[index_i + 1];

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

