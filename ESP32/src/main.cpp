#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

//WebSocket server
//WebSockets rely on HTTP for the initial handshake, but after that, 
//client sends a special HTTP request called a WebSocket upgrade request, upgrading into WebSocket protocol, which is independent of HTTP
AsyncWebSocket socket("/ws"); //Handles WebSocket-specific communication
AsyncWebServer gate(80); // This is the main HTTP server instance

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Client connected: %u\n", client->id());
    client->text("Welcome to ESP32 WebSocket Server!");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("Client disconnected: %u\n", client->id());
  } else if (type == WS_EVT_DATA) {
    String command = "";
    for (size_t i = 0; i < len; i++) {
      command += (char)data[i];
    }
    Serial.printf("Command received: %s\n", command.c_str());

    if (command == GO) {
      Serial.println(GO);
    } else if (command == BACK) {
      Serial.println(BACK);
    } else {
      Serial.println("Unknown command");
    }
    client->text("Command received: " + command);
  }
}

void setup() {
  //Heartbeat
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); //default uart0

  //Wifi
  const char* ssid = WIFI__SSID;
  const char* password = WIFI_PASSWORD;
  bool WiFi_Status = WiFi_INIT(ssid, password);

  //WebSocket
  socket.onEvent(onWebSocketEvent);
  gate.addHandler(&socket);
  gate.begin();
  if (WiFi_Status = true){
    Serial.println("Bootup successful");
  }
  //Uart1.begin(115200, SERIAL_7E1, 9, 10); //uart1 word length: 8 bits including parity
}

void loop() {
  HeartBeat();
  socket.cleanupClients();
}



