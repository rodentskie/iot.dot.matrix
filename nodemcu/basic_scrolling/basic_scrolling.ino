#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

#define MAX_DEVICES 4
#define CS_PIN 15

MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

char msg[120];



void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("Basic scrolling text");
  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();
  strcpy (msg, "WE ARE TEST!");
  Display.displayScroll(msg, PA_RIGHT, PA_SCROLL_LEFT, 150);
}

void loop() {
  if (Display.displayAnimate()) {
    Display.displayReset();
  }
}
