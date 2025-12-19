#include "sensor.hpp"

Sensor::Sensor() :
    sensorSignal(0),
    lastSignal(0),
    threshold(550),  // Default threshold value
    beatDetected(false),
    lastBeatTime(0),
    bpm(0),
    peakValue(0),
    troughValue(4095),
    pulseDetected(false),
    peakDecayRate(2),
    troughDecayRate(2),
    offset(0) {  // Default offset value
}

void Sensor::init() {
  pinMode(PULSE_INPUT, INPUT);
  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}

void Sensor::update() {
  sensorSignal = analogRead(PULSE_INPUT) + offset;  // Apply offset to sensor signal
  beatDetected = false;

  // Track peaks and troughs for reference (but threshold is now manually set)
  if (sensorSignal > peakValue) {
    peakValue = sensorSignal;
  }
  if (sensorSignal < troughValue) {
    troughValue = sensorSignal;
  }

  // Threshold is now manually configurable - no auto-adjustment
  // threshold = (peakValue + troughValue) / 2;

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
  peakValue -= peakDecayRate;
  troughValue += troughDecayRate;
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

// Decay rate configuration methods
void Sensor::setPeakDecayRate(int rate) {
  peakDecayRate = max(PEAK_DECAY_MIN, min(PEAK_DECAY_MAX, rate));
}

void Sensor::setTroughDecayRate(int rate) {
  troughDecayRate = max(TROUGH_DECAY_MIN, min(TROUGH_DECAY_MAX, rate));
}

int Sensor::getPeakDecayRate() const {
  return peakDecayRate;
}

int Sensor::getTroughDecayRate() const {
  return troughDecayRate;
}

// Offset configuration methods
void Sensor::setOffset(int value) {
  offset = max(OFFSET_MIN, min(OFFSET_MAX, value));
}

int Sensor::getOffset() const {
  return offset;
}

// Threshold configuration methods
void Sensor::setThreshold(int value) {
  threshold = max(THRESHOLD_MIN, min(THRESHOLD_MAX, value));
}

int Sensor::getThreshold() const {
  return threshold;
}