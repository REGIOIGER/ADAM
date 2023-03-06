#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ArduinoJson.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define LED PA8

bool rx_done = false;

Adafruit_BME680 bme; // I2C

StaticJsonDocument<200> doc;

double round_value(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("HoverGames 3"));
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
  
#if defined(WISBLOCK_BASE_5005) || defined(WISBLOCK_BASE_5005_O)
  api.system.sleep.setup(RUI_WAKEUP_FALLING_EDGE, WB_IO1);  // Set a pin to be wake up pin
#else
#warning Please set the right pin refer to the documentation
  api.system.sleep.setup(RUI_WAKEUP_FALLING_EDGE, 0xFF);
#endif

  Serial.println("RAKwireless LoRaWan P2P Example");
  Serial.println("------------------------------------------------------");
  Serial.printf("Set Node device work mode %s\r\n", api.lorawan.nwm.set(0) ? "Success" : "Fail");
  Serial.printf("Set P2P mode frequency %s\r\n", api.lorawan.pfreq.set(915000000) ? "Success" : "Fail");
  Serial.printf("Set P2P mode spreading factor %s\r\n", api.lorawan.psf.set(12) ? "Success" : "Fail");
  Serial.printf("Set P2P mode bandwidth %s\r\n", api.lorawan.pbw.set(0) ? "Success" : "Fail"); // 125
  Serial.printf("Set P2P mode code rate %s\r\n", api.lorawan.pcr.set(0) ? "Success" : "Fail"); // 4/5
  Serial.printf("Set P2P mode preamble length %s\r\n", api.lorawan.ppl.set(8) ? "Success" : "Fail");
  Serial.printf("Set P2P mode tx power %s\r\n", api.lorawan.ptp.set(22) ? "Success" : "Fail");

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms


}

void writeSensorData(String& output)
{
  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(5);
  StaticJsonDocument<capacity> doc;

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }  
  
  doc["sensor"] = "bme688";
  
  JsonArray data = doc.createNestedArray("data");
    
  data.add(round_value(bme.temperature));
  data.add(round_value(bme.pressure / 100.0));
  data.add(round_value(bme.humidity));
  data.add(round_value(bme.gas_resistance / 1000.0));
  data.add(round_value(bme.readAltitude(SEALEVELPRESSURE_HPA)));

  serializeJson(doc, output);
}

void loop()
{
  //uint8_t payload[] = "HELL ";
  bool send_result = false;

  String output;
  writeSensorData(output);
  Serial.println(output);
  uint8_t len = output.length();
  Serial.println(len);
  
  Serial.println();
  
  send_result = api.lorawan.psend(len, (uint8_t *)output.c_str());
  Serial.printf("P2P send %s\r\n", send_result ? "Success" : "Fail");

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(200);                       // wait for a second

  api.system.sleep.all(300000);
  //delay(5000);
}
