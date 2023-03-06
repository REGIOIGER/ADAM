#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>

//define the pins used by the transceiver module
#define ss D8
#define rst D0
#define dio0 D2

String message = "";
String msgLoRa = "";

StaticJsonDocument<200> doc;

int recLoRa = 0;
int recespnow = 0;

uint8_t broadcastAddress[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

typedef struct struct_message {
    float AQI;
    float t;
    float h;
    float p;
    float g;
} struct_message;

// Create a struct_message to hold incoming
struct_message incoming;

double round_value(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}

void writeSensorData(String& output)
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(5);
  StaticJsonDocument<capacity> doc;
  
  doc["sensor"] = "bme688";
  
  JsonArray data = doc.createNestedArray("data");
    
  data.add(round_value(incoming.AQI));
  data.add(round_value(incoming.t));
  data.add(round_value(incoming.h));
  data.add(round_value(incoming.p));
  data.add(round_value(incoming.g));

  serializeJson(doc, output);
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incoming, incomingData, sizeof(incoming));
  Serial.print("Received ");
  Serial.print(len);
  Serial.println(" bytes");
  
  msgLoRa = "";
  writeSensorData(msgLoRa);
  Serial.println(msgLoRa);

  uint8_t l = msgLoRa.length();
  Serial.println(l);

  recespnow = 1;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(12);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    message = "";
    recLoRa = 1;
    // read packet
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }
    Serial.print(message);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.println();
  }

  if(recLoRa) {

    Serial.print("Sending packet: ");
  
    // send packet
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
    recLoRa = 0;
  }

  if(recespnow) {

    Serial.print("Sending packet: ");
  
    // send packet
    LoRa.beginPacket();
    LoRa.print(msgLoRa);
    LoRa.endPacket();
    recespnow = 0;
  }
  
}
