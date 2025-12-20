#pragma once

#include <Arduino.h>

class Joystick {
private:
    // Joystick pin definitions
    const int JOY_UP_PIN;
    const int JOY_DOWN_PIN;
    const int JOY_LEFT_PIN;
    const int JOY_RIGHT_PIN;
    const int JOY_MID_PIN;

    // State tracking for edge detection
    bool lastUpState;
    bool lastDownState;
    bool lastLeftState;
    bool lastRightState;
    bool lastMidState;

    // Current debounced states
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
    bool midPressed;

    // Edge detection states (true until cleared by wasPressed functions)
    bool upWasPressed;
    bool downWasPressed;
    bool leftWasPressed;
    bool rightWasPressed;
    bool midWasPressed;

    // Last debounce times
    unsigned long lastUpDebounce;
    unsigned long lastDownDebounce;
    unsigned long lastLeftDebounce;
    unsigned long lastRightDebounce;
    unsigned long lastMidDebounce;

    // Debug output control
    bool debugOutput;

    // Debounce delay in milliseconds
    const unsigned long DEBOUNCE_DELAY;

public:
    Joystick();
    void init();
    void update();

    // Button controls and getters
    bool isUpPressed();
    bool isDownPressed();
    bool isLeftPressed();
    bool isRightPressed();
    bool isMidPressed();

    bool wasUpPressed();
    bool wasDownPressed();
    bool wasLeftPressed();
    bool wasRightPressed();
    bool wasMidPressed();

    // Debug output control
    bool getDebugOutput() const;
    void setDebugOutput(bool enable);
};
