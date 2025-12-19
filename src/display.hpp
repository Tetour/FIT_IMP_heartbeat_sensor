#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sensor.hpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class Display {
private:
    enum class MenuOption : int {
        OFFSET = 0,
        BEAT_THRESHOLD,
        PEAK_DECAY,
        TROUGH_DECAY,
        NUM_OF_OPTIONS
    };

    MenuOption currentSelection;
    Adafruit_SSD1306 display;
    
    int offsetValue;
    // Min/Max accessed via Sensor::getOffsetMin/Max()
    static const int offsetStep = 1;

    int thresholdValue;
    // Min/Max accessed via Sensor::getThresholdMin/Max()
    static const int thresholdStep = 5;

    int peakDecayValue;
    // Min/Max accessed via Sensor::getPeakDecayMin/Max()
    static const int peakDecayStep = 1;

    int troughDecayValue;
    // Min/Max accessed via Sensor::getTroughDecayMin/Max()
    static const int troughDecayStep = 1;

    // Helper method for menu display
    const char* getPrefix(MenuOption option) const;

public:
    Display();
    void init();
    void showBPM(int bpm);

    // Menu display and navigation methods
    void showMenu();
    void handleUpMovement();
    void handleDownMovement();
    void handleLeftMovement();   // Decrease current setting value
    void handleRightMovement();  // Increase current setting value
    
    // Getters for sensor configuration
    int getOffsetValue() const { return offsetValue; }
    int getThresholdValue() const { return thresholdValue; }
    int getPeakDecayValue() const { return peakDecayValue; }
    int getTroughDecayValue() const { return troughDecayValue; }

    // Sync display values with sensor values
    void syncWithSensor(const Sensor& sensor);
};
