#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
        DynamicJsonDocument doc(1024);
        doc["latest"] = true;
        String output;
        serializeJson(doc, output);
        webSocket.sendTXT(output);
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

  webSocket.begin("172.17.254.5", 3000, "/");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}
