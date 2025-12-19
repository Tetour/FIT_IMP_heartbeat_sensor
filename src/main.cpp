#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"

Display display;
bool showMenu = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  display.init();
  initJoystick();
  initSensor();
}

void loop() {
  updateJoystick();
  updateSensor();

  // Toggle menu on middle button press (edge detection)
  if (wasMidPressed()) {
    showMenu = !showMenu;
    if (Serial) {
      Serial.print("Menu ");
      Serial.println(showMenu ? "shown" : "hidden");
    }
  }

  // Update display every 100ms
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 100) {
    if (showMenu) {
      display.showMenu();
    } else {
      int currentBPM = getBPM();
      display.showBPM(currentBPM);
    }
    lastDisplayUpdate = millis();
  }

  delay(20);
}
