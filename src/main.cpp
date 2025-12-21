#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"
#include "data_logger.hpp"

DataLogger dataLogger;
Sensor sensor(dataLogger);
Display display(sensor, dataLogger);
Joystick joystick;

bool showMenu = false;

void setup() {
  Serial.begin(115200);
  
  display.init();
  joystick.init();
  dataLogger.init();
  sensor.init();
}

void loop() {
  joystick.update();
  sensor.update();

  // Toggle settings on middle button press
  if (joystick.wasMidPressed()) {
    showMenu = !showMenu;
  }

  // Settings navigation
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
      if (dataLogger.isRecording()) {
        dataLogger.stopRecording();
      } else {
        dataLogger.startRecording("/sensor_data.csv");
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
