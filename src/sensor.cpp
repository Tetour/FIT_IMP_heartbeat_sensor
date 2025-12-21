#include "sensor.hpp"
#include <SPIFFS.h>

Sensor::Sensor(DataLogger& logger) :
    sensorSignal(0),
    lastSignal(0),
    thresholdOffset(DEFAULT_THRESHOLD_OFFSET),  // Default threshold value
    beatDetected(false),
    lastBeatTime(0),
    bpm(0),
    peakValue(0),
    troughValue(4095),
    effectiveThreshold(0),
    pulseDetected(false),
    peakDecayRate(DEFAULT_PEAK_DECAY_RATE),
    troughDecayRate(DEFAULT_TROUGH_DECAY_RATE),
    bpmOffset(DEFAULT_BPM_OFFSET),
    debugOutput(false),
    dataLogger(logger) {
}

void Sensor::init() {
  pinMode(PULSE_INPUT, INPUT);

  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}void Sensor::update() {
  sensorSignal = analogRead(PULSE_INPUT);  // Read raw sensor signal
  
  // Maintain signal history for console smoothing (keep only last 3)
  signalHistory.push_back(sensorSignal);
  if (signalHistory.size() > 3) {
    signalHistory.erase(signalHistory.begin());
  }
  
  beatDetected = false;

  // Track peaks and troughs for adaptive threshold
  if (sensorSignal > peakValue) {
    peakValue = sensorSignal;
  }
  if (sensorSignal < troughValue) {
    troughValue = sensorSignal;
  }

  // Auto-adjust threshold based on peak and trough
  // Use midpoint between peak and trough as the threshold
  int autoThreshold = (peakValue + troughValue) / 2;
  
  // Apply manual offset to the auto threshold
  effectiveThreshold = autoThreshold + thresholdOffset;

  // Detect rising edge (beat)
  if (sensorSignal > effectiveThreshold && lastSignal <= effectiveThreshold) {
    unsigned long now = millis();
    unsigned long timeSinceLastBeat = now - lastBeatTime;

    // Valid beat if at least 300ms since last beat (max 200 BPM)
    if (timeSinceLastBeat > 300) {
      beatDetected = true;
      lastBeatTime = now;

      // Store beat timestamp (keep only last 11 for 10 intervals)
      beatTimestamps.push_back(now);
      if (beatTimestamps.size() > 11) {
        beatTimestamps.erase(beatTimestamps.begin());
      }
    }
  }

  // Decay peaks and troughs slowly for auto-adjustment
  peakValue -= peakDecayRate;
  troughValue += troughDecayRate;
  if (peakValue < sensorSignal) peakValue = sensorSignal;
  if (troughValue > sensorSignal) troughValue = sensorSignal;

  // Comprehensive debug output every 100ms to avoid flooding
  static unsigned long lastDebugTime = 0;
  if (debugOutput && Serial && millis() - lastDebugTime > 100) {
    Serial.print("Signal: ");
    Serial.printf("%4d", getSignal());
    Serial.print(" Peak: ");
    Serial.printf("%4d", peakValue);
    Serial.print(" Trough: ");
    Serial.printf("%4d", troughValue);
    Serial.print(" AutoThreshold: ");
    Serial.printf("%4d", autoThreshold);
    Serial.print(" Offset: ");
    Serial.print(thresholdOffset);
    Serial.print(" EffectiveThreshold: ");
    Serial.printf("%4d", effectiveThreshold);
    Serial.print(" PulseState: ");
    Serial.print(pulseDetected ? "HIGH" : "LOW ");
    Serial.print(" BPM: ");
    Serial.println(getBPM());
    lastDebugTime = millis();
  }

  lastSignal = sensorSignal;
}

int Sensor::getBPM() {
  // Need at least 2 beats to calculate BPM
  if (beatTimestamps.size() < 2) {
    return 0;
  }
  
  // Calculate BPM for each interval and average them
  int numIntervals = beatTimestamps.size() - 1;
  int validBpmCount = 0;
  int bpmSum = 0;
  
  for (int i = 0; i < numIntervals; i++) {
    unsigned long interval = beatTimestamps[i + 1] - beatTimestamps[i];
    if (interval > 0) {
      int bpm = 60000 / interval;
      bpmSum += bpm;
      validBpmCount++;
    }
  }
  
  int averageBPM = bpmSum / validBpmCount;
  
  // Apply BPM offset
  return max(0, averageBPM + bpmOffset);
}

bool Sensor::isBeatDetected() {
  return beatDetected;
}

int Sensor::getSignal() {
  return sensorSignal;
}

int Sensor::getSmoothedSignal() {
  if (signalHistory.empty()) {
    return sensorSignal;
  }
  
  int sum = 0;
  for (int signal : signalHistory) {
    sum += signal;
  }
  
  return sum / signalHistory.size();
}

int Sensor::getPeakValue() const {
  return peakValue;
}

int Sensor::getTroughValue() const {
  return troughValue;
}

int Sensor::getEffectiveThreshold() const {
  return effectiveThreshold;
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

// BPM offset configuration methods
void Sensor::setBpmOffset(int value) {
  bpmOffset = max(BPM_OFFSET_MIN, min(BPM_OFFSET_MAX, value));
}

int Sensor::getBpmOffset() const {
  return bpmOffset;
}

// Threshold offset configuration methods
void Sensor::setThresholdOffset(int value) {
  thresholdOffset = max(THRESHOLD_OFFSET_MIN, min(THRESHOLD_OFFSET_MAX, value));
}

int Sensor::getThresholdOffset() const {
  return thresholdOffset;
}

// Debug output configuration methods
void Sensor::setDebugOutput(bool enable) {
  debugOutput = enable;
}

bool Sensor::getDebugOutput() const {
  return debugOutput;
}
