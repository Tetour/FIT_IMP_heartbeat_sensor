#pragma once

#include <Arduino.h>
#include <vector>

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
    
    // BPM smoothing over 100 beats
    std::vector<int> bpmHistory;
    
    // Signal smoothing for console output over 3 values
    std::vector<int> signalHistory;
    
    // Configuration parameters
    int peakDecayRate;
    int troughDecayRate;
    int bpmOffset;
    int thresholdOffset;

    // Configuration parameter defaults
    static const int DEFAULT_BPM_OFFSET = 0;
    static const int DEFAULT_THRESHOLD_OFFSET = 0;
    static const int DEFAULT_PEAK_DECAY_RATE = 2;
    static const int DEFAULT_TROUGH_DECAY_RATE = 2;

    // Configuration parameter limits
    static const int BPM_OFFSET_MIN = -50;
    static const int BPM_OFFSET_MAX = 50;
    static const int THRESHOLD_OFFSET_MIN = -500;
    static const int THRESHOLD_OFFSET_MAX = 500;
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
    int  getSmoothedSignal();     // Get smoothed signal value for console output
    
    // Sensor configuration
    int  getBpmOffset() const;
    void setBpmOffset(int value);

    int  getThresholdOffset() const;
    void setThresholdOffset(int value);

    int  getPeakDecayRate() const;
    void setPeakDecayRate(int rate);

    int  getTroughDecayRate() const;
    void setTroughDecayRate(int rate);

    // Configuration limits
    static int getBpmOffsetMin() { return BPM_OFFSET_MIN; }
    static int getBpmOffsetMax() { return BPM_OFFSET_MAX; }

    static int getThresholdOffsetMin() { return THRESHOLD_OFFSET_MIN; }
    static int getThresholdOffsetMax() { return THRESHOLD_OFFSET_MAX; }

    static int getPeakDecayMin() { return PEAK_DECAY_MIN; }
    static int getPeakDecayMax() { return PEAK_DECAY_MAX; }

    static int getTroughDecayMin() { return TROUGH_DECAY_MIN; }
    static int getTroughDecayMax() { return TROUGH_DECAY_MAX; }

};
