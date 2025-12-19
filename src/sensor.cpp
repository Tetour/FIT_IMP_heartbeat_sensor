#include "sensor.hpp"

Sensor::Sensor() :
    sensorSignal(0),
    lastSignal(0),
    threshold(THRESHOLD),
    beatDetected(false),
    lastBeatTime(0),
    bpm(0),
    peakValue(0),
    troughValue(4095),
    pulseDetected(false) {
}

void Sensor::init() {
  pinMode(PULSE_INPUT, INPUT);
  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}

void Sensor::update() {
  sensorSignal = analogRead(PULSE_INPUT);
  beatDetected = false;

  // Track peaks and troughs for auto-threshold adjustment
  if (sensorSignal > peakValue) {
    peakValue = sensorSignal;
  }
  if (sensorSignal < troughValue) {
    troughValue = sensorSignal;
  }

  // Auto-adjust threshold (midpoint between peak and trough)
  threshold = (peakValue + troughValue) / 2;

  // Detect rising edge (beat)
  if (sensorSignal > threshold && lastSignal <= threshold && !pulseDetected) {
    unsigned long now = millis();
    unsigned long timeSinceLastBeat = now - lastBeatTime;

    // Valid beat if at least 300ms since last beat (max 200 BPM)
    if (timeSinceLastBeat > 300) {
      beatDetected = true;
      pulseDetected = true;

      // Calculate BPM
      bpm = 60000 / timeSinceLastBeat;

      // Clamp BPM to reasonable range
      if (bpm < 40) bpm = 0;
      if (bpm > 200) bpm = 0;

      lastBeatTime = now;

      if (Serial && bpm > 0) {
        Serial.print("BPM: ");
        Serial.print(bpm);
        Serial.print(" Signal: ");
        Serial.println(sensorSignal);
      }
    }
  }

  // Reset pulse detection on falling edge
  if (sensorSignal < threshold) {
    pulseDetected = false;
  }

  // Decay peaks and troughs slowly for auto-adjustment
  peakValue -= 2;
  troughValue += 2;
  if (peakValue < sensorSignal) peakValue = sensorSignal;
  if (troughValue > sensorSignal) troughValue = sensorSignal;

  lastSignal = sensorSignal;
}

int Sensor::getBPM() {
  // Return 0 if no beat detected in last 3 seconds
  if (millis() - lastBeatTime > 3000) {
    return 0;
  }
  return bpm;
}

bool Sensor::isBeatDetected() {
  return beatDetected;
}

int Sensor::getSignal() {
  return sensorSignal;
}