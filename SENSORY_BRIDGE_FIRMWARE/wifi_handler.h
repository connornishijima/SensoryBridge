bool init_wifi() {
  WiFiManager wm;
  bool res;

  for (int i = 0; i < 17; i = i + 8) {
    chip_id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  String ssid = "Sensory_Bridge_";
  ssid += String(chip_id);

  return wm.autoConnect(ssid.c_str(), "s3ns0ryB");
}

void discovery_check_in() {
  uint8_t base_mac[6];

  // Get MAC address for WiFi station
  esp_read_mac(base_mac, ESP_MAC_WIFI_STA);

  char mac_add[18] = { 0 };
  sprintf(mac_add, "%02X:%02X:%02X:%02X:%02X:%02X", base_mac[0], base_mac[1], base_mac[2], base_mac[3], base_mac[4], base_mac[5]);

  HTTPClient http;
  String url = "http://app.sensorybridge.rocks/discovery/?type=check_in&local_ip=" + WiFi.localIP().toString() + "&mac=" + String(mac_add);
  http.begin(url);  //HTTP
  int http_code = http.GET();

  if (http_code > 0) {
    if (http_code == HTTP_CODE_OK) {
      USBSerial.println("CHECKED IN");
    } else {
      USBSerial.println("FAILED TO CHECK IN");
    }
  }

  http.end();
}

void wifi_loop() {
  webSocket.loop();
}

void hexdump(const void* mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*)mem;
  USBSerial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
      USBSerial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    USBSerial.printf("%02X ", *src);
    src++;
  }
  USBSerial.printf("\n");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USBSerial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        USBSerial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      char input_buf[64];
      sprintf(input_buf, "%s", payload);
      if (strcmp(input_buf, "SB?") == 0) {
        webSocket.sendTXT(num, "SB!");
      }
      
      else if (strcmp(input_buf, "SB_IDENT") == 0) {
        FastLED.setBrightness(255);  // "current_brightness" still holds user-set brightness
        for (uint8_t i = 0; i < 2; i++) {
          for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
            leds_out[i] = CRGB(32, 16, 0);
          }
          FastLED.show();
          ledcWrite(0, 2048);
          ledcWrite(1, 2048);
          ledcWrite(2, 2048);
          delay(200);
          for (uint16_t i = 0; i < STRIP_LED_COUNT; i++) {
            leds_out[i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          ledcWrite(0, 0);
          ledcWrite(1, 0);
          ledcWrite(2, 0);
          delay(200);
        }
        FastLED.setBrightness(current_brightness);
      }
      
      else if (strcmp(input_buf, "GET_MAC") == 0) {
        uint8_t base_mac[6];

        // Get MAC address for WiFi station
        esp_read_mac(base_mac, ESP_MAC_WIFI_STA);

        char mac_add[32] = { 0 };
        sprintf(mac_add, "SBMAC|%02X:%02X:%02X:%02X:%02X:%02X", base_mac[0], base_mac[1], base_mac[2], base_mac[3], base_mac[4], base_mac[5]);
        webSocket.sendTXT(num, mac_add);
      }

      else if (strcmp(input_buf, "GET_VER") == 0) {
        String ver = String("SBVER|") + String(FIRMWARE_VERSION);
        webSocket.sendTXT(num, ver);
      }

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      USBSerial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}