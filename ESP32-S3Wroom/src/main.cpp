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

void Forward();
void Forward(){
  digitalWrite(T5, HIGH);
  digitalWrite(T6, LOW);
  digitalWrite(T9, HIGH);
  digitalWrite(T10, LOW);
}

void StopForward();
void StopForward(){
  digitalWrite(T5, LOW);
  digitalWrite(T6, LOW);
  digitalWrite(T9, LOW);
  digitalWrite(T10, LOW);
}

void Back();
void Back(){
  digitalWrite(T5, LOW);                     
  digitalWrite(T6, HIGH);                                                                                                                                                                                                                                                                        
  digitalWrite(T9, LOW);
  digitalWrite(T10, HIGH);
}

void StopBack();
void StopBack(){
  digitalWrite(T5, LOW);
  digitalWrite(T6, LOW);
  digitalWrite(T9, LOW);
  digitalWrite(T10, LOW);
}

void Right();
void Right(){
  digitalWrite(T5, HIGH);
  digitalWrite(T6, LOW);
  digitalWrite(T9, LOW);
  digitalWrite(T10, HIGH);
}

void StopRight();
void StopRight(){
  digitalWrite(T5, LOW);
  digitalWrite(T6, LOW);
  digitalWrite(T9, LOW);
  digitalWrite(T10, LOW);
}

void Left();
void Left(){
  digitalWrite(T5, LOW);
  digitalWrite(T6, HIGH);
  digitalWrite(T9, HIGH);
  digitalWrite(T10, LOW);
}

void StopLeft();
void StopLeft(){
  digitalWrite(T5, LOW);
  digitalWrite(T6, LOW);
  digitalWrite(T9, LOW);
  digitalWrite(T10, LOW);
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
//client sends a special HTTP request called a WebSocket upgrade request, upgrading into WebSocket protocol, which is independent of HTTP
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
    if(command == "w1"){
      Forward();
    }
    else if (command == "w0"){
      StopForward();
    }
    else if (command == "s1"){
      Back();
    }
    else if (command == "s0"){
      StopBack();
    }
    else if (command == "d1"){
      Right();
    }
    else if (command == "d0"){
      StopRight();
    }
    else if (command == "a1"){
      Left();
    }
    else if (command == "a0"){
      StopLeft();
    }
    Serial.println(command);
  }
}
/*
// Camera
void handleStream(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginChunkedResponse(
        "multipart/x-mixed-replace; boundary=frame",
        [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
            camera_fb_t *fb = esp_camera_fb_get();
            if (!fb) {
                Serial.println("Camera capture failed");
                return 0;
            }
            size_t len = fb->len;
            memcpy(buffer, fb->buf, len);
            esp_camera_fb_return(fb);
            return len;
        });
    response->addHeader("Cache-Control", "no-store");
    request->send(response);
}
*/

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

  //Motor Controls
  pinMode(T7, OUTPUT); //ENB
  analogWrite(T7, 255); //0 - 255
  pinMode(T5, OUTPUT);  //Forward IN/OUT 3
  pinMode(T6, OUTPUT); //Backward In/OUT 4

  pinMode(T11, OUTPUT); //ENA
  analogWrite(T11, 255); //0-225
  pinMode(T9, OUTPUT); //Right IN/OUT 1
  pinMode(T10, OUTPUT); //Left IN/OUT 2
  /*
  // Camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = 4;
  config.pin_d1 = 5;
  config.pin_d2 = 18;
  config.pin_d3 = 19;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sccb_sda = 21;
  config.pin_sccb_scl = 22;
  config.pin_pwdn = -1;  // Not used
  config.pin_reset = -1; // Not used
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA; // 1600x1200
        config.jpeg_quality = 10;          // Lower number = higher quality
        config.fb_count = 2;               // Double buffer
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
  

    // Setup web server routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Go to /stream to view the video feed.");
    });
    server.on("/stream", HTTP_GET, handleStream);
    server.begin();
    Serial.println("HTTP server started.");
  */

  
}
void loop() {
}



