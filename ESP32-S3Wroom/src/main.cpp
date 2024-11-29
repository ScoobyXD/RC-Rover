#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp_camera.h"

//Heartbeat
void HeartBeat();

void HeartBeat() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

//Wifi
void WiFi_INIT(const char* ssid, const char* password);

void WiFi_INIT(const char* ssid, const char* password){
  Serial.println("Connection sequence starting now");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to establish a connection");
    delay(1000); // for some reason it is incredibly important that this stays at 1000.
  }
  Serial.println("Connected to the WiFi network: " + WiFi.SSID());
  Serial.print("Local ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

}

//WebSocket server
AsyncWebServer server(80); // This is the main HTTP server instance
AsyncWebSocket socket("/ws"); //Handles WebSocket-specific communication

//WebSockets rely on HTTP for the initial handshake, but after that, 
//client sends a special HTTP requesDWWWWWDWt called a WebSocket upgrade request, upgrading into WebSocket protocol, which is independent of HTTP
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Client connected: %u\n", client->id());
    client->text("Connected with ESP32");
  } 
  else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("Client disconnected: %u\n", client->id());
  } 
  else if (type == WS_EVT_DATA) {
    String command = "";
    for (size_t i = 0; i < len; i++) {
      command += (char)data[i];
    }
    Serial.println(command);
  }
}

//Motor Commands


void setup() {
  //Heartbeat
  delay(1000);
  Serial.begin(115200); //default uart0
  pinMode(LED_BUILTIN, OUTPUT);

  //Wifi
  const char* ssid = WIFI_SSID;
  const char* password = WIFI_PASSWORD;
  WiFi_INIT(ssid, password);

  //WebSocket
  socket.onEvent(onWebSocketEvent);
  server.addHandler(&socket); //add the socket to the server
  server.begin();
  Serial.println("Websocket server has started");

  //Success message
  Serial.println("Bootup complete");
  //Uart1.begin(115200, SERIAL_7E1, 9, 10); //uart1 word length: 8 bits including parity
}

void loop() {
//test commit
}



