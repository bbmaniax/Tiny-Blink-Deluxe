// Blink-KAI

#include <Arduino.h>

#include "Button.h"
#include "DebugSerial.h"

// #define RP2040
#define ATTINY

#if defined(RP2040)
#define BUTTON_PIN 29
#define LED_PIN 14
#elif defined(ATTINY)
#define BUTTON_PIN 3
#define LED_PIN 1
#endif

#define LED_ON HIGH
#define LED_OFF LOW

#define LED_WAIT 500
#define LED_WAIT_FAST 100
#define LED_WAIT_SLOW 1000

Button button(BUTTON_PIN);

enum ViewMode {
  VIEWMODE_BLINK,
  VIEWMODE_BLINK_FAST,
  VIEWMODE_BLINK_SLOW,
  VIEWMODE_COUNT
} viewMode = VIEWMODE_BLINK;

void setup() {
  DEBUG_SERIAL_BEGIN(9600);
  DEBUG_SERIAL_WAIT_FOR();
  DEBUG_SERIAL_PRINTLN();
  DEBUG_SERIAL_PRINTLN("--");
  DEBUG_SERIAL_PRINTLN("Blink-Deluxe");

  pinMode(LED_PIN, OUTPUT);
  button.begin();
}

void loop() {
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();

  static bool ledOn = true;
  static unsigned long ledWait = LED_WAIT;

  button.update();
  if (button.isClicked()) {
    DEBUG_SERIAL_PRINTLN("Button clicked");
    viewMode = static_cast<ViewMode>((viewMode + 1) % VIEWMODE_COUNT);
    currentMillis = 0;
    ledOn = true;
    switch (viewMode) {
      case VIEWMODE_BLINK: ledWait = LED_WAIT; break;
      case VIEWMODE_BLINK_FAST: ledWait = LED_WAIT_FAST; break;
      case VIEWMODE_BLINK_SLOW: ledWait = LED_WAIT_SLOW; break;
      default: break;
    }
    DEBUG_SERIAL_PRINTLN("New view mode=" + String(static_cast<int>(viewMode)) + ", ledWait=" + String(ledWait));
  }

  if (currentMillis - lastMillis > ledWait) {
    // DEBUG_SERIAL_PRINTLN("Time to toggle LED");
    digitalWrite(LED_PIN, ledOn ? LED_ON : LED_OFF);
    ledOn = !ledOn;
    lastMillis = currentMillis;
  }

  delay(10);
}
