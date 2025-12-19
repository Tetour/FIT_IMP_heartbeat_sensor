#include "sensor.hpp"

// Variables for beat detection
static int sensorSignal = 0;
static int lastSignal = 0;
static int threshold = THRESHOLD;
static bool beatDetected = false;
static unsigned long lastBeatTime = 0;
static int bpm = 0;
static int peakValue = 0;
static int troughValue = 4095;
static bool pulseDetected = false;

void initSensor() {
  pinMode(PULSE_INPUT, INPUT);
  if (Serial) {
    Serial.println("Pulse sensor initialized on GPIO 34");
  }
}

void updateSensor() {
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
        Serial.print("♥ Beat! BPM: ");
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

int getBPM() {
  // Return 0 if no beat detected in last 3 seconds
  if (millis() - lastBeatTime > 3000) {
    return 0;
  }
  return bpm;
}

bool isBeatDetected() {
  return beatDetected;
}

int getSignal() {
  return sensorSignal;
}