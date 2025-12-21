#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sensor.hpp"
#include "data_logger.hpp"

class Display {
private:
    enum class MenuOption : int {
        DATA_RECORDING = 0,
        AUTORECORDING,
        BEAT_THRESHOLD,
        DECAY_RATE,
        NUM_OF_OPTIONS
    };

    // Display constants
    static const int SCREEN_WIDTH = 128;
    static const int SCREEN_HEIGHT = 64;
    static const int OLED_RESET = -1;

    MenuOption currentSelection;
    Adafruit_SSD1306 display;
    Sensor& sensor;
    DataLogger& dataLogger;
    bool debugOutput;  // Debug output control
    
    static const int offsetStep = 5;
    static const int thresholdStep = 5;
    static const int decayStep = 1;
    static const int recordingStep = 5;

    // Helper method for menu display
    const char* getPrefix(MenuOption option) const;

public:
    Display(Sensor& sensorRef, DataLogger& loggerRef);
    void init();
    void showBPM(int bpm);

    // Menu display and navigation methods
    void showMenu();
    void handleUpMovement();
    void handleDownMovement();
    void handleLeftMovement();   // Decrease current setting value
    void handleRightMovement();  // Increase current setting value

    // Debug output control
    void setDebugOutput(bool enable);
    bool getDebugOutput() const;

};
