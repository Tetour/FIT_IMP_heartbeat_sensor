#pragma once

#include <Arduino.h>

#define PULSE_INPUT 34        // Analog input pin (GPIO 34 is ADC1_CH6)
#define THRESHOLD 550         // Threshold for beat detection

class Sensor {
private:
    // Variables for beat detection
    int sensorSignal;
    int lastSignal;
    int threshold;
    bool beatDetected;
    unsigned long lastBeatTime;
    int bpm;
    int peakValue;
    int troughValue;
    bool pulseDetected;

public:
    Sensor();
    void init();
    void update();
    int  getBPM();                // Get current BPM
    bool isBeatDetected();        // Check if a heartbeat was just detected
    int  getSignal();             // Get raw sensor signal value
};
