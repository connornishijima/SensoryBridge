void init_usb() {
  USBSerial.manufacturer("Lixie Labs");
  USBSerial.product("Sensory Bridge");
  USBSerial.deviceID(0x1209, 0xABED); // Cool, cool cool cool
  USBSerial.revision(100);

  if (!USBSerial.begin()){
    Serial.println("Failed to start CDC USB device");
  }
}