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
  chip_id = ESP.getEfuseMac(); // Get the chip ID as a uint64_t
  
  chip_id_high = uint64_t(chip_id >> 32); // Split the chip ID into two uint32_t values
  chip_id_low  = chip_id;
  
  char hex_string[9]; // ... Create a char array to store the hex string
  bytes_32 b; // ........... Create a bytes_32 struct to hold the bytes of chip_id_high
  
  b.long_val = chip_id_high; // .................. Assign chip_id_high to the long_val member of the struct
  bytes_to_hex_string(b.bytes, hex_string); // ... Convert the bytes of chip_id_high to a zero-padded hex string
  USBSerial.print(hex_string); // ................... Print the hex string to the serial port
  
  USBSerial.print('_'); //  Print an underscore separator
  
  b.long_val = chip_id_low; // ................... Assign chip_id_low to the long_val member of the struct
  bytes_to_hex_string(b.bytes, hex_string); // ... Convert the bytes of chip_id_low to a zero-padded hex string
  USBSerial.print(hex_string); // ................... Print the hex string to the serial port
  
  USBSerial.println(); // Print a newline character
}
