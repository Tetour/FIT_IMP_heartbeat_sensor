#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"

bool showMenu = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  initDisplay();
  initJoystick();
  initSensor();
}

void loop() {
  updateJoystick();
  updateSensor();

  showMenu = isMidPressed() ? !showMenu : showMenu;

  // Update display every 100ms
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 100) {
    if (showMenu) {
      displayMenu();
    } else {
      int currentBPM = getBPM();
      displayBPM(currentBPM);
    }
    lastDisplayUpdate = millis();
  }

  delay(20);
}
