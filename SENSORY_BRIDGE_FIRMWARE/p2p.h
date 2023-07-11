/*----------------------------------------
  Sensory Bridge P2P NETWORK FUNCTIONS
  ----------------------------------------*/

// Fully documenting the P2P functions is a TODO for now.
// Sorry!

const uint8_t broadcast_address[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
esp_now_peer_info_t broadcast_peer;
bool flashing_flag = false;

enum COMMAND_TYPES {
  COMMAND_NULL,               // 0
  COMMAND_SYNC_SETTINGS,      // 1
  COMMAND_TRIGGER_NOISE_CAL,  // 2
  COMMAND_CLEAR_NOISE_CAL,    // 3
  COMMAND_IDENTIFY_MAIN,      // 4
  NUM_COMMAND_TYPES
};

struct SB_COMMAND_SYNC_SETTINGS {
  char ident[4] = { 'S', 'B', 'C', 0 };
  uint8_t command_type = COMMAND_SYNC_SETTINGS;
  float   PHOTONS_KNOB;
  float   CHROMA_KNOB;
  float   MOOD_KNOB;
  uint8_t LIGHTSHOW_MODE;
  uint8_t MIRROR_ENABLED;
  uint8_t CHROMAGRAM_RANGE;
};

struct SB_COMMAND_TRIGGER_NOISE_CAL {
  char ident[4] = { 'S', 'B', 'C', 0 };
  uint8_t command_type = COMMAND_TRIGGER_NOISE_CAL;
};

struct SB_COMMAND_CLEAR_NOISE_CAL {
  char ident[4] = { 'S', 'B', 'C', 0 };
  uint8_t command_type = COMMAND_CLEAR_NOISE_CAL;
};

struct SB_COMMAND_IDENTIFY_MAIN {
  char ident[4] = { 'S', 'B', 'C', 0 };
  uint8_t command_type = COMMAND_IDENTIFY_MAIN;
};

void print_mac(const uint8_t *mac_addr) {
  USBSerial.print(mac_addr[0], HEX);
  USBSerial.print(':');
  USBSerial.print(mac_addr[1], HEX);
  USBSerial.print(':');
  USBSerial.print(mac_addr[2], HEX);
  USBSerial.print(':');
  USBSerial.print(mac_addr[3], HEX);
  USBSerial.print(':');
  USBSerial.print(mac_addr[4], HEX);
  USBSerial.print(':');
  USBSerial.print(mac_addr[5], HEX);
}

void sync_settings(uint32_t t_now) {
  SB_COMMAND_SYNC_SETTINGS setting;

  setting.PHOTONS_KNOB = CONFIG.PHOTONS;
  setting.CHROMA_KNOB = CONFIG.CHROMA;
  setting.MOOD_KNOB = CONFIG.MOOD;
  setting.LIGHTSHOW_MODE = CONFIG.LIGHTSHOW_MODE;
  setting.MIRROR_ENABLED = CONFIG.MIRROR_ENABLED;
  setting.CHROMAGRAM_RANGE = CONFIG.CHROMAGRAM_RANGE;

  const uint8_t *peer_addr = broadcast_peer.peer_addr;
  esp_now_send(peer_addr, (uint8_t *)&setting, sizeof(SB_COMMAND_SYNC_SETTINGS));
}

void identify_main_unit() {
  USBSerial.println("[IDENTIFY MAIN UNIT]");
  SB_COMMAND_IDENTIFY_MAIN identify;

  const uint8_t *peer_addr = broadcast_peer.peer_addr;
  for (uint8_t i = 0; i < 4; i++) {
    esp_now_send(peer_addr, (uint8_t *)&identify, sizeof(SB_COMMAND_IDENTIFY_MAIN));
  }

  CRGB16 col = {1.0, 0.0, 0.0};
  blocking_flash(col); // We aren't main unit, flash red
}

void propagate_noise_cal() {
  if (CONFIG.IS_MAIN_UNIT) {
    SB_COMMAND_TRIGGER_NOISE_CAL trigger;
    const uint8_t *peer_addr = broadcast_peer.peer_addr;
    for (uint8_t i = 0; i < 4; i++) {
      esp_now_send(peer_addr, (uint8_t *)&trigger, sizeof(SB_COMMAND_TRIGGER_NOISE_CAL));
    }
  }
}

void propagate_noise_reset() {
  if (CONFIG.IS_MAIN_UNIT) {
    SB_COMMAND_CLEAR_NOISE_CAL clear;
    const uint8_t *peer_addr = broadcast_peer.peer_addr;
    for (uint8_t i = 0; i < 4; i++) {
      esp_now_send(peer_addr, (uint8_t *)&clear, sizeof(SB_COMMAND_CLEAR_NOISE_CAL));
    }
  }
}

void on_data_tx(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // nothing
}

void on_data_rx(const uint8_t *mac_addr, const uint8_t *incoming_data, int len) {
  //Serial.println("RX PACKET");
  main_override = false;
  last_rx_time = millis();
  char data_type[4];
  memcpy(data_type, incoming_data, 4);

  if (strcmp(data_type, "SBC") == 0) {
    uint8_t command_type = incoming_data[4];

    if (debug_mode) {
      USBSerial.print("RX COMMAND OF TYPE ");
      USBSerial.print(command_type);
      USBSerial.print(" FROM ");
      print_mac(mac_addr);
      USBSerial.println();
    }

    if (command_type == COMMAND_SYNC_SETTINGS) {
      if (CONFIG.IS_MAIN_UNIT == false) {
        SB_COMMAND_SYNC_SETTINGS settings;
        memcpy(&settings, incoming_data, sizeof(SB_COMMAND_SYNC_SETTINGS));

        CONFIG.PHOTONS = settings.PHOTONS_KNOB;
        CONFIG.CHROMA = settings.CHROMA_KNOB;
        CONFIG.MOOD = settings.MOOD_KNOB;
        CONFIG.LIGHTSHOW_MODE = settings.LIGHTSHOW_MODE;
        CONFIG.MIRROR_ENABLED = settings.MIRROR_ENABLED;
        CONFIG.CHROMAGRAM_RANGE = settings.CHROMAGRAM_RANGE;
      }
    } else if (command_type == COMMAND_TRIGGER_NOISE_CAL) {
      if (CONFIG.IS_MAIN_UNIT == false) {
        if (noise_complete == true) {
          noise_complete = false;
          start_noise_cal();
        }
      }
    } else if (command_type == COMMAND_CLEAR_NOISE_CAL) {
      if (CONFIG.IS_MAIN_UNIT == false) {
        if (noise_complete == true) {
          clear_noise_cal();
        }
      }
    } else if (command_type == COMMAND_IDENTIFY_MAIN) {  // MAIN UNIT ACCEPTS THIS COMMAND FROM OTHERS
      if (CONFIG.IS_MAIN_UNIT) {
        flashing_flag = true;
      }
    }
  }
  else {
    if (debug_mode) {
      USBSerial.print("UNKNOWN PACKET:");
      USBSerial.println(data_type);
    }
  }
}

void init_p2p() {
  WiFi.mode(WIFI_MODE_STA);

  USBSerial.print("ESP-NOW INIT: ");
  USBSerial.println(esp_err_to_name(esp_now_init()));

  esp_now_register_send_cb(on_data_tx);
  esp_now_register_recv_cb(on_data_rx);

  memset(&broadcast_peer, 0, sizeof(broadcast_peer));
  memcpy(broadcast_peer.peer_addr, broadcast_address, 6);
  broadcast_peer.channel = 0; // TODO - avoid broadcast mode to allow for other WIFI channels (Promiscuous only works on channel 0)
  broadcast_peer.encrypt = false;

  USBSerial.print("ESP-NOW ADD BROADCAST PEER: ");
  USBSerial.println(esp_err_to_name(esp_now_add_peer(&broadcast_peer)));
}

void run_p2p() {
  uint32_t t_now = millis();

  if (t_now - last_rx_time >= 1000) {
    main_override = true;
  }

  if (CONFIG.IS_MAIN_UNIT) {
    sync_settings(t_now);
  }

  if (flashing_flag) {
    flashing_flag = false;
    CRGB16 col = {0.0, 1.0, 0.0};
    blocking_flash(col);
  }
}
