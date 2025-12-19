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
