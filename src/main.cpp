#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>

int bytes_read = 0;
int counter = 0;
esp_now_peer_info_t peerInfo = {};
uint8_t cameraAddress[] = {0x24, 0x6F, 0x28, 0xB3, 0x2E, 0x7C};
uint8_t PMK[] = { 0x01, 0x14, 0x93, 0x08, 0x27, 0x93, 0x12, 0x26, 0x91, 0x10, 0x14, 0x94, 0x03, 0x25, 0x93, 0x00 };
uint8_t LMK[] = { 0x01, 0x14, 0x93, 0x08, 0x27, 0x93, 0x12, 0x26, 0x91, 0x10, 0x14, 0x94, 0x03, 0x25, 0x93, 0x01 };

void dataRecv(const uint8_t * mac, const uint8_t *val, int s) { 
  if(s == 1) {
    switch(*val) {
      case 0x01:
        Serial.println("STX");
        bytes_read = 0;
        counter = 0;
        break;
      case 0x02:
        Serial.println("ETX");
        break;
      default:
        break;
    }
  } else {
    for(int i=0; i<s; i++) {
      if(val[i] < 0x10) {
        Serial.print("0");
      }
      Serial.print(val[i], HEX);
    }
    bytes_read += s;
    counter += 1;
  }
}

// Camera MAC - 24:6F:28:B3:2E:7C
// Receiver MAC - A4:CF:12:60:F4:98
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW Initialized");

    esp_now_set_pmk((uint8_t *)PMK);

    for (uint8_t i = 0; i < 16; i++) {
      peerInfo.lmk[i] = LMK[i];
    }

    peerInfo.channel = 0;
    peerInfo.encrypt = true;
    memcpy(peerInfo.peer_addr, cameraAddress, 6);

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }

    if(esp_now_register_recv_cb(dataRecv) == ESP_OK) {
      Serial.println("Callback registered");
    } else {
      Serial.println("Failed registering callback");
    }
  }
}

void loop() {

}