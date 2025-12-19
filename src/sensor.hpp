#pragma once

#include <Arduino.h>

class Sensor {
private:
    // Hardware configuration
    static const int PULSE_INPUT = 34;        // Analog input pin (GPIO 34 is ADC1_CH6)
    
    // Variables for beat detection
    int sensorSignal;
    int lastSignal;
    unsigned long lastBeatTime;
    int bpm;
    int peakValue;
    int troughValue;
    bool beatDetected;
    bool pulseDetected;
    
    // Configuration parameters
    int peakDecayRate;
    int troughDecayRate;
    int offset;
    int threshold;

    // Configuration parameter defaults
    static const int DEFAULT_THRESHOLD = 550;
    static const int DEFAULT_PEAK_DECAY_RATE = 2;
    static const int DEFAULT_TROUGH_DECAY_RATE = 2;
    static const int DEFAULT_OFFSET = 0;

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
    int  getValueOffset() const;
    void setValueOffset(int value);

    int  getThresholdOffset() const;
    void setThresholdOffset(int value);

    int  getPeakDecayRate() const;
    void setPeakDecayRate(int rate);

    int  getTroughDecayRate() const;
    void setTroughDecayRate(int rate);

    // Configuration limits
    static int getValueOffsetMin() { return OFFSET_MIN; }
    static int getValueOffsetMax() { return OFFSET_MAX; }

    static int getThresholdOffsetMin() { return THRESHOLD_MIN; }
    static int getThresholdOffsetMax() { return THRESHOLD_MAX; }

    static int getPeakDecayMin() { return PEAK_DECAY_MIN; }
    static int getPeakDecayMax() { return PEAK_DECAY_MAX; }

    static int getTroughDecayMin() { return TROUGH_DECAY_MIN; }
    static int getTroughDecayMax() { return TROUGH_DECAY_MAX; }

};
