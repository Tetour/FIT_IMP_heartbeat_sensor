#pragma once

#include <Arduino.h>

#define PULSE_INPUT 34        // Analog input pin (GPIO 34 is ADC1_CH6)
#define THRESHOLD 550         // Threshold for beat detection

class Sensor {
private:
    // Variables for beat detection
    int sensorSignal;
    int lastSignal;
    bool beatDetected;
    unsigned long lastBeatTime;
    int bpm;
    int peakValue;
    int troughValue;
    bool pulseDetected;
    
    // Configuration parameters
    int peakDecayRate;
    int troughDecayRate;
    int offset;
    int threshold;

    // Configuration parameter limits
    static const int PEAK_DECAY_MIN = 0;
    static const int PEAK_DECAY_MAX = 100;
    static const int TROUGH_DECAY_MIN = 0;
    static const int TROUGH_DECAY_MAX = 100;
    static const int OFFSET_MIN = 0;
    static const int OFFSET_MAX = 100;
    static const int THRESHOLD_MIN = 0;
    static const int THRESHOLD_MAX = 1023;

public:
    Sensor();
    void init();
    void update();
    int  getBPM();                // Get current BPM
    bool isBeatDetected();        // Check if a heartbeat was just detected
    int  getSignal();             // Get raw sensor signal value
    
    // Sensor configuration
    int  getPeakDecayRate() const;
    void setPeakDecayRate(int rate);

    int  getTroughDecayRate() const;
    void setTroughDecayRate(int rate);

    int  getOffset() const;
    void setOffset(int value);

    int  getThreshold() const;
    void setThreshold(int value);

    // Configuration limits
    static int getPeakDecayMin() { return PEAK_DECAY_MIN; }
    static int getPeakDecayMax() { return PEAK_DECAY_MAX; }
    static int getTroughDecayMin() { return TROUGH_DECAY_MIN; }
    static int getTroughDecayMax() { return TROUGH_DECAY_MAX; }
    static int getOffsetMin() { return OFFSET_MIN; }
    static int getOffsetMax() { return OFFSET_MAX; }
    static int getThresholdMin() { return THRESHOLD_MIN; }
    static int getThresholdMax() { return THRESHOLD_MAX; }
};
