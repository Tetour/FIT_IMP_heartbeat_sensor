#include "sensor.hpp"
#include <PulseSensorPlayground.h>

PulseSensorPlayground pulseSensor;

void initSensor() {
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setThreshold(THRESHOLD);
  
  if (pulseSensor.begin()) {
    if (Serial) {
      Serial.println("PulseSensor initialized!");
    }
  } else {
    if (Serial) {
      Serial.println("PulseSensor initialization failed!");
    }
  }
}

void updateSensor() {
  int myBPM = pulseSensor.getBeatsPerMinute();
  
  if (pulseSensor.sawStartOfBeat()) {
    if (Serial) {
      Serial.print("♥ Beat detected! BPM: ");
      Serial.println(myBPM);
    }
  }
}

int getBPM() {
  return pulseSensor.getBeatsPerMinute();
}

bool isBeatDetected() {
  return pulseSensor.sawStartOfBeat();
}

int getSignal() {
  return pulseSensor.getLatestSample();
}