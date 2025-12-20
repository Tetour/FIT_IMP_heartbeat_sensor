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
    int valueOffset;
    int thresholdOffset;

    // Configuration parameter defaults
    static const int DEFAULT_VALUE_OFFSET = 0;
    static const int DEFAULT_THRESHOLD_OFFSET = 550;
    static const int DEFAULT_PEAK_DECAY_RATE = 2;
    static const int DEFAULT_TROUGH_DECAY_RATE = 2;

    // Configuration parameter limits
    static const int VALUE_OFFSET_MIN = -1000;
    static const int VALUE_OFFSET_MAX = 1000;
    static const int THRESHOLD_OFFSET_MIN = 0;
    static const int THRESHOLD_OFFSET_MAX = 1023;
    static const int PEAK_DECAY_MIN = 0;
    static const int PEAK_DECAY_MAX = 100;
    static const int TROUGH_DECAY_MIN = 0;
    static const int TROUGH_DECAY_MAX = 100;

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
    static int getValueOffsetMin() { return VALUE_OFFSET_MIN; }
    static int getValueOffsetMax() { return VALUE_OFFSET_MAX; }

    static int getThresholdOffsetMin() { return THRESHOLD_OFFSET_MIN; }
    static int getThresholdOffsetMax() { return THRESHOLD_OFFSET_MAX; }

    static int getPeakDecayMin() { return PEAK_DECAY_MIN; }
    static int getPeakDecayMax() { return PEAK_DECAY_MAX; }

    static int getTroughDecayMin() { return TROUGH_DECAY_MIN; }
    static int getTroughDecayMax() { return TROUGH_DECAY_MAX; }

};
