#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


#define MAX_DEVICES 4
#define CS_PIN 15
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define USE_SERIAL Serial

int LED = 5;

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

char msg[120];

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      {
        USE_SERIAL.printf("[WSc] Disconnected!\n");
        digitalWrite(LED, LOW);
      }
      break;
    case WStype_CONNECTED:
      {
        digitalWrite(LED, HIGH);
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
        DynamicJsonDocument doc(1024);
        doc["latest"] = true;
        String output;
        serializeJson(doc, output);
        webSocket.sendTXT(output);
      }
      break;
    case WStype_TEXT:
      {
        USE_SERIAL.printf("[WSc] get text: %s\n", payload);
        char* ca = (char*)payload;
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, ca);
        if (!doc["msgs"]) return;
        strcpy (msg, doc["msgs"]);
      }
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

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println("Basic IoT scrolling text");

  pinMode(LED, OUTPUT);

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP("Wee", "84753620A");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();

  strcpy (msg, "DEFAULT MESSAGE.");
  Display.displayScroll(msg, PA_RIGHT, PA_SCROLL_LEFT, 150);

  //  webSocket.begin("172.17.254.8", 3000, "/");

  webSocket.beginSSL("api.npcdavao.com", 443, "/");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
  if (Display.displayAnimate()) {
    Display.displayReset();
  }
}
