#include <Arduino.h>
#include "display.hpp"
#include "joystick.hpp"
#include "sensor.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  initDisplay();
  printHelloWorld();
  initJoystick();
  initSensor();
}

void loop() {
  updateJoystick();
  updateSensor();
  delay(20);
}
