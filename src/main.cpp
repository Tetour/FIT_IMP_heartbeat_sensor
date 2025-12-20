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
  Serial.println("Starting...");
  
  display.init();
  joystick.init();
  sensor.init();
}

void loop() {
  joystick.update();
  sensor.update();

  // Check for serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "DUMP") {
      Serial.println("Dumping recorded data...");
      sensor.dumpRecordedData();
    } else if (command == "HELP") {
      Serial.println("Available commands:");
      Serial.println("  DUMP - Output recorded data to serial");
      Serial.println("  HELP - Show this help message");
    }
  }

  if (sensor.isRecording()) {
    // Indicate recording status on Serial Monitor
    static unsigned long lastRecordIndicator = 0;
    if (millis() - lastRecordIndicator > 2000) {
      Serial.println("Recording sensor data...");
      lastRecordIndicator = millis();
    }
  }

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
      if (Serial) {
        Serial.println("Menu: LEFT movement (decrease value)");
      }
    }
    if (joystick.wasRightPressed()) {
      display.handleRightMovement();
      if (Serial) {
        Serial.println("Menu: RIGHT movement (increase value)");
      }
    }
  } else {
    // Direct recording toggle when not in menu (BPM display mode)
    if (joystick.wasLeftPressed() || joystick.wasRightPressed()) {
      if (sensor.isRecording()) {
        sensor.stopRecording();
        if (Serial) {
          Serial.println("Recording stopped via joystick");
        }
      } else {
        sensor.startRecording("/sensor_data.csv");
        if (Serial) {
          Serial.println("Recording started via joystick");
        }
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
