#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sensor.hpp"

class Display {
private:
    enum class MenuOption : int {
        BPM_OFFSET = 0,
        BEAT_THRESHOLD,
        PEAK_DECAY,
        TROUGH_DECAY,
        NUM_OF_OPTIONS
    };

    // Display constants
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = -1;

    MenuOption currentSelection;
    Adafruit_SSD1306 display;
    Sensor& sensor;
    
    static const int offsetStep = 1;
    static const int thresholdStep = 5;
    static const int peakDecayStep = 1;
    static const int troughDecayStep = 1;

    // Helper method for menu display
    const char* getPrefix(MenuOption option) const;

public:
    Display(Sensor& sensorRef);
    void init();
    void showBPM(int bpm);

    // Menu display and navigation methods
    void showMenu();
    void handleUpMovement();
    void handleDownMovement();
    void handleLeftMovement();   // Decrease current setting value
    void handleRightMovement();  // Increase current setting value
};
