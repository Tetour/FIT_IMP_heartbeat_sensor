#include "sensor.hpp"

Sensor::Sensor() :
    sensorSignal(0),
    lastSignal(0),
    thresholdOffset(DEFAULT_THRESHOLD_OFFSET),  // Default threshold value
    beatDetected(false),
    lastBeatTime(0),
    bpm(0),
    peakValue(0),
    troughValue(4095),
    pulseDetected(false),
    peakDecayRate(DEFAULT_PEAK_DECAY_RATE),
    troughDecayRate(DEFAULT_TROUGH_DECAY_RATE),
    valueOffset(DEFAULT_VALUE_OFFSET) {  // Default offset value
}

void Sensor::init() {
  pinMode(PULSE_INPUT, INPUT);
  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}

void Sensor::update() {
  sensorSignal = analogRead(PULSE_INPUT) + valueOffset;  // Apply offset to sensor signal
  beatDetected = false;

  // Track peaks and troughs for reference (but threshold is now manually set)
  if (sensorSignal > peakValue) {
    peakValue = sensorSignal;
  }
  if (sensorSignal < troughValue) {
    troughValue = sensorSignal;
  }

  // Threshold is now manually configurable - no auto-adjustment
  // thresholdOffset is set manually via menu

  // Detect rising edge (beat)
  if (sensorSignal > thresholdOffset && lastSignal <= thresholdOffset && !pulseDetected) {
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
  if (sensorSignal < thresholdOffset) {
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

// Peak decay rate configuration methods
void Sensor::setPeakDecayRate(int rate) {
  peakDecayRate = max(PEAK_DECAY_MIN, min(PEAK_DECAY_MAX, rate));
}

int Sensor::getPeakDecayRate() const {
  return peakDecayRate;
}

// Trough decay rate configuration methods
void Sensor::setTroughDecayRate(int rate) {
  troughDecayRate = max(TROUGH_DECAY_MIN, min(TROUGH_DECAY_MAX, rate));
}

int Sensor::getTroughDecayRate() const {
  return troughDecayRate;
}

// Value offset configuration methods
void Sensor::setValueOffset(int value) {
  valueOffset = max(VALUE_OFFSET_MIN, min(VALUE_OFFSET_MAX, value));
}

int Sensor::getValueOffset() const {
  return valueOffset;
}

// Threshold offset configuration methods
void Sensor::setThresholdOffset(int value) {
  thresholdOffset = max(THRESHOLD_OFFSET_MIN, min(THRESHOLD_OFFSET_MAX, value));
}

int Sensor::getThresholdOffset() const {
  return thresholdOffset;
}