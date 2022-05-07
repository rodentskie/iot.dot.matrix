#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define USE_SERIAL Serial

int buttonPin = 4;
int connectionLedPin = 15;
int sendLedPin = 5;
bool trigger = false;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
        digitalWrite(connectionLedPin, HIGH);
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[WSc] get text: %s\n", payload);
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);
      break;
    case WStype_PING:
      USE_SERIAL.printf("[WSc] get ping\n");
      break;
    case WStype_PONG:
      USE_SERIAL.printf("[WSc] get pong\n");
      break;
  }

}

void setup() {
  USE_SERIAL.begin(115200);

  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP("Wee", "84753620A");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(connectionLedPin, OUTPUT);
  pinMode(sendLedPin, OUTPUT);

  //  webSocket.begin("172.17.254.8", 3000, "/");

  webSocket.beginSSL("api.npcdavao.com", 443, "/");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
  int sensorVal = digitalRead(buttonPin);
  sendToWebSocketServer(sensorVal);
}


void sendToWebSocketServer (int sensorVal) {
  if (sensorVal == HIGH) {
    if (trigger == false) {
      digitalWrite(sendLedPin, LOW);
      trigger = true;
      delay(10);
    }
  } else {
    if (trigger == true) {
      digitalWrite(sendLedPin, HIGH);
      trigger = false;

      DynamicJsonDocument doc(1024);
      doc["isEmit"] = true;
      doc["msg"] = "HELLO FROM OTHER NODEMCU";
      String output;
      serializeJson(doc, output);
      webSocket.sendTXT(output);

      delay(10);
    }
  }
}
