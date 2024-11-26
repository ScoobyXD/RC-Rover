#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to establish a connection");
    delay(1000);
  }
  Serial.println("WiFi connection established!");
  Serial.print("Local ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

//WebSocket server
//WebSockets rely on HTTP for the initial handshake, but after that, 
//client sends a special HTTP request called a WebSocket upgrade request, upgrading into WebSocket protocol, which is independent of HTTP
AsyncWebServer server(80); // This is the main HTTP server instance
AsyncWebSocket socket("/ws"); //Handles WebSocket-specific communication

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Client connected: %u\n", client->id());
    client->text("Connected with ESP32");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("Client disconnected: %u\n", client->id());
  } else if (type == WS_EVT_DATA) {
    String command = "";
    for (size_t i = 0; i < len; i++) {
      command += (char)data[i];
    }
    Serial.printf("Command received: %s\n", command.c_str());
    if(command = GO)
    {
      Serial.printf(GO);
    }
    else if(command = BACK)
    {
      Serial.printf(BACK);
    }
    else if(command = RIGHT)
    {
      Serial.printf(RIGHT);
    }
    else if(command = LEFT)
    {
      Serial.printf(LEFT);
    }
    else {
      Serial.printf(UNKNOWN_COMMAND);
    }
      //respond to client
      client->text("ESP32 received the command: " + command);
  }
}

void setup() {
  //Heartbeat
  Serial.begin(115200); //default uart0
  pinMode(LED_BUILTIN, OUTPUT);

  //Wifi
  const char* ssid = WIFI__SSID;
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
  HeartBeat();
  socket.cleanupClients();
}



