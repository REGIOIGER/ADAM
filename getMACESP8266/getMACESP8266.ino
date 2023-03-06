#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(9600);
  delay(3000);
  Serial.println();
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
