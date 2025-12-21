#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"

Sensor sensor;
Display display(sensor);
Joystick joystick;

bool showMenu = false;

void setup() {
  Serial.begin(115200);
  
  display.init();
  joystick.init();
  sensor.init();
}

void loop() {
  joystick.update();
  sensor.update();

  // Toggle menu on middle button press (edge detection)
  if (joystick.wasMidPressed()) {
    showMenu = !showMenu;
  }

  // Menu navigation when menu is shown
  if (showMenu) {
    if (joystick.wasUpPressed()) {
      display.handleUpMovement();
    }
    if (joystick.wasDownPressed()) {
      display.handleDownMovement();
    }
    if (joystick.wasLeftPressed()) {
      display.handleLeftMovement();
    }
    if (joystick.wasRightPressed()) {
      display.handleRightMovement();
    }
  } else {
    // Direct recording toggle when in BPM display mode
    if (joystick.wasLeftPressed() || joystick.wasRightPressed()) {
      if (sensor.isRecording()) {
        sensor.stopRecording();
      } else {
        sensor.startRecording();
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

  delay(50);
}
