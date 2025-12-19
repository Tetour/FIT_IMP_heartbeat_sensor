#pragma once

#include <Arduino.h>

#define JOY_UP    18
#define JOY_DOWN  19
#define JOY_LEFT  23
#define JOY_RIGHT 5
#define JOY_MID   13

#define DEBOUNCE_DELAY 50  // milliseconds

class Joystick {
private:
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

public:
    Joystick();
    void init();
    void update();  // Call this in loop()

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
};
