#include "config.h"
#include <Arduino.h>
#include <WiFi.h>


//Heartbeat
void HeartBeat();

void HeartBeat() {
  Serial.println("I'm working");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

//Wifi
bool WiFi_INIT(const char* ssid, const char* password);

bool WiFi_INIT(const char* ssid, const char* password){
  Serial.println("Connection sequence starting now");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to establish a connection");
    delay(1000);
  }
  Serial.println("WiFi connection established!");
  Serial.println("Local ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  return true;
}



void setup() {
  //Heartbeat
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); //default uart0

  //Wifi
  const char* ssid = WIFI__SSID;
  const char* password = WIFI_PASSWORD;
  bool WiFi_Status = WiFi_INIT(ssid, password);

  if (WiFi_Status = true){
    Serial.println("Bootup successful");
  }
  //Uart1.begin(115200, SERIAL_7E1, 9, 10); //uart1 word length: 8 bits including parity
}

void loop() {
  HeartBeat();
}



