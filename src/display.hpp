#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class Display {
private:
    enum class MenuOption : int {
        OFFSET = 0,
        THRESHOLD,
        PEAK_DECAY,
        TROUGH_DECAY,
        NUM_OF_OPTIONS
    };

    MenuOption currentSelection;
    Adafruit_SSD1306 display;
    
    int offsetValue;
    static const int offsetMin = 0;
    static const int offsetMax = 100;
    static const int offsetStep = 1;

    int thresholdValue;
    static const int thresholdMin = 0;
    static const int thresholdMax = 255;
    static const int thresholdStep = 5;

    int peakDecayValue;
    static const int peakDecayMin = 0;
    static const int peakDecayMax = 100;
    static const int peakDecayStep = 1;

    int troughDecayValue;
    static const int troughDecayMin = 0;
    static const int troughDecayMax = 100;
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
};
