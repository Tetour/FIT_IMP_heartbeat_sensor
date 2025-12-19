#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"

Display display;
Joystick joystick;
Sensor sensor;

bool showMenu = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  display.init();
  joystick.init();
  sensor.init();
  
  // Sync display values with sensor defaults
  display.syncWithSensor(sensor);
}

void loop() {
  joystick.update();
  sensor.update();

  // Toggle menu on middle button press (edge detection)
  if (joystick.wasMidPressed()) {
    showMenu = !showMenu;
    if (Serial) {
      Serial.print("Menu ");
      Serial.println(showMenu ? "shown" : "hidden");
    }
  }

  // Menu navigation when menu is shown
  if (showMenu) {
    if (joystick.wasUpPressed()) {
      display.handleUpMovement();
      if (Serial) {
        Serial.println("Menu: UP movement");
      }
    }
    if (joystick.wasDownPressed()) {
      display.handleDownMovement();
      if (Serial) {
        Serial.println("Menu: DOWN movement");
      }
    }
    if (joystick.wasLeftPressed()) {
      display.handleLeftMovement();
      // Update sensor with new values
      sensor.setOffset(display.getOffsetValue());
      sensor.setThreshold(display.getThresholdValue());
      sensor.setPeakDecayRate(display.getPeakDecayValue());
      sensor.setTroughDecayRate(display.getTroughDecayValue());
      if (Serial) {
        Serial.println("Menu: LEFT movement (decrease value)");
      }
    }
    if (joystick.wasRightPressed()) {
      display.handleRightMovement();
      // Update sensor with new values
      sensor.setOffset(display.getOffsetValue());
      sensor.setThreshold(display.getThresholdValue());
      sensor.setPeakDecayRate(display.getPeakDecayValue());
      sensor.setTroughDecayRate(display.getTroughDecayValue());
      if (Serial) {
        Serial.println("Menu: RIGHT movement (increase value)");
      }
    }
  }

  // Update display every 100ms
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 100) {
    if (showMenu) {
      display.showMenu();
    } else {
      int currentBPM = sensor.getBPM();
      display.showBPM(currentBPM);
    }
    lastDisplayUpdate = millis();
  }

  delay(20);
}
