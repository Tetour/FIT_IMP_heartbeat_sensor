#include "sensor.hpp"
#include <SPIFFS.h>

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
    bpmOffset(DEFAULT_BPM_OFFSET),  // Default BPM offset value
    recordingEnabled(false),  // Default recording state
    debugOutput(false) {  // Default debug output disabled
}

void Sensor::init() {
  pinMode(PULSE_INPUT, INPUT);
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    if (Serial) {
      Serial.println("SPIFFS initialization failed!");
    }
  } else {
    if (Serial) {
      Serial.println("SPIFFS initialized successfully");
    }
  }
  
  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}

void Sensor::update() {
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
  int effectiveThreshold = autoThreshold + thresholdOffset;

  // Detect rising edge (beat)
  if (sensorSignal > effectiveThreshold && lastSignal <= effectiveThreshold && !pulseDetected) {
    unsigned long now = millis();
    unsigned long timeSinceLastBeat = now - lastBeatTime;

    // Valid beat if at least 300ms since last beat (max 200 BPM)
    if (timeSinceLastBeat > 300) {
      beatDetected = true;
      pulseDetected = true;

      // Calculate BPM
      int currentBPM = 60000 / timeSinceLastBeat;

      // Clamp BPM to reasonable range
      if (currentBPM < 40) currentBPM = 0;
      if (currentBPM > 200) currentBPM = 0;

      // Store BPM in history for smoothing (keep only last 100)
      bpmHistory.push_back(currentBPM);
      if (bpmHistory.size() > 100) {
        bpmHistory.erase(bpmHistory.begin());
      }

      lastBeatTime = now;
    }
  }

  // Reset pulse detection on falling edge
  if (sensorSignal < effectiveThreshold) {
    pulseDetected = false;
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
    Serial.print(getSmoothedSignal());
    Serial.print(" Peak: ");
    Serial.print(peakValue);
    Serial.print(" Trough: ");
    Serial.print(troughValue);
    Serial.print(" AutoThreshold: ");
    Serial.print(autoThreshold);
    Serial.print(" Offset: ");
    Serial.print(thresholdOffset);
    Serial.print(" EffectiveThreshold: ");
    Serial.print(effectiveThreshold);
    Serial.print(" PulseState: ");
    Serial.print(pulseDetected ? "HIGH" : "LOW");
    Serial.print(" BPM: ");
    Serial.println(getBPM());
    lastDebugTime = millis();
  }

  // Save data to file if recording is enabled
  if (recordingEnabled) {
    saveSignalData(nullptr);  // Use stored filename
  }

  lastSignal = sensorSignal;
}

int Sensor::getBPM() {
  // Return 0 if no beat detected in last 3 seconds
  if (millis() - lastBeatTime > 3000) {
    return 0;
  }
  
  // Calculate average BPM over available history
  if (bpmHistory.empty()) {
    return 0;
  }
  
  int sum = 0;
  int validCount = 0;
  for (int bpmValue : bpmHistory) {
    if (bpmValue > 0) {  // Only count valid BPM readings
      sum += bpmValue;
      validCount++;
    }
  }
  
  int averageBPM = validCount > 0 ? sum / validCount : 0;
  
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

void Sensor::saveSignalData(const char* filename) {
  // Use stored filename if none provided
  const char* actualFilename = filename ? filename : recordingFilename.c_str();
  
  if (!actualFilename || strlen(actualFilename) == 0) {
    return;  // Silent fail - avoid console spam
  }
  
  // Open file for appending
  File file = SPIFFS.open(actualFilename, FILE_APPEND);
  if (!file) {
    // Only report error once per session to avoid console spam
    static bool errorReported = false;
    if (!errorReported && Serial) {
      Serial.print("Failed to open file: ");
      Serial.println(actualFilename);
      errorReported = true;
    }
    return;
  }

  // Write timestamp and signal data (CSV format)
  unsigned long timestamp = millis();
  file.print(timestamp);
  file.print(",");
  file.print(sensorSignal);
  file.print(",");
  file.print(peakValue);
  file.print(",");
  file.print(troughValue);
  file.print(",");
  file.print(getBPM());
  file.print(",");
  file.println(beatDetected ? "1" : "0");

  file.close();
  
  // No console output here - would spam serial at 50Hz
}

void Sensor::startRecording(const char* filename) {
  recordingFilename = filename;
  
  // Delete existing file and create new one with CSV header
  if (SPIFFS.exists(filename)) {
    SPIFFS.remove(filename);
    if (Serial) {
      Serial.println("Cleared existing data file");
    }
  }
  
  // Create file and write CSV header
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (file) {
    file.println("timestamp,signal,peak,trough,bpm,beat_detected");
    file.close();
    
    recordingEnabled = true;
    
    if (Serial) {
      Serial.print("Started recording data to: ");
      Serial.println(filename);
      Serial.println("NOTE: Data is saved to ESP32 SPIFFS filesystem");
    }
  } else {
    if (Serial) {
      Serial.print("Failed to create recording file: ");
      Serial.println(filename);
    }
  }
}

void Sensor::stopRecording() {
  recordingEnabled = false;
  
  if (Serial) {
    Serial.println("Stopped recording data");
    Serial.println("Auto-dumping data...");
  }
  
  // Automatically dump recorded data when stopping
  dumpRecordedData();
}

bool Sensor::isRecording() const {
  return recordingEnabled;
}

void Sensor::dumpRecordedData() {
  if (!recordingFilename.length()) {
    if (Serial) {
      Serial.println("ERROR: No recording filename set");
    }
    return;
  }

  File file = SPIFFS.open(recordingFilename.c_str(), FILE_READ);
  if (!file) {
    if (Serial) {
      Serial.print("ERROR: Failed to open file for reading: ");
      Serial.println(recordingFilename);
    }
    return;
  }

  if (Serial) {
    Serial.println("===DATA_START===");
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (Serial) {
      Serial.println(line);
    }
  }

  file.close();

  if (Serial) {
    Serial.println("===DATA_END===");
  }
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