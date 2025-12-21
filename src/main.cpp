#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"
#include "data_logger.hpp"

DataLogger dataLogger;
Sensor sensor(dataLogger);
Display display(sensor, dataLogger);
Joystick joystick;

enum class ScreenState {
    BPM_DISPLAY,
    SIGNAL_DISPLAY,
    SETTINGS_MENU
};

ScreenState currentScreen = ScreenState::BPM_DISPLAY;

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

  // Toggle screens on middle button press
  if (joystick.wasMidPressed()) {
    switch (currentScreen) {
      case ScreenState::BPM_DISPLAY:
        currentScreen = ScreenState::SIGNAL_DISPLAY;
        break;
      case ScreenState::SIGNAL_DISPLAY:
        currentScreen = ScreenState::SETTINGS_MENU;
        break;
      case ScreenState::SETTINGS_MENU:
        currentScreen = ScreenState::BPM_DISPLAY;
        break;
    }
  }

  // Settings navigation (only when in settings menu)
  if (currentScreen == ScreenState::SETTINGS_MENU) {
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
  } else if (currentScreen == ScreenState::BPM_DISPLAY || currentScreen == ScreenState::SIGNAL_DISPLAY) {
    // Direct recording toggle when in BPM display or Signal display modes
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
    switch (currentScreen) {
      case ScreenState::BPM_DISPLAY:
        {
          int currentBPM = sensor.getBPM();
          display.showBPM(currentBPM);
        }
        break;
      case ScreenState::SIGNAL_DISPLAY:
        display.showHelloWorld();
        break;
      case ScreenState::SETTINGS_MENU:
        display.showMenu();
        break;
    }
    lastDisplayUpdate = millis();
  }

  delay(20);
}
