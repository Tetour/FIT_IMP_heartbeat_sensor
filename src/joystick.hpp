#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <Arduino.h>

#define JOY_UP    18
#define JOY_DOWN  19
#define JOY_LEFT  23
#define JOY_RIGHT 5
#define JOY_MID   13

#define DEBOUNCE_DELAY 50  // milliseconds

void initJoystick();
void updateJoystick();  // Call this in loop()
bool isUpPressed();
bool isDownPressed();
bool isLeftPressed();
bool isRightPressed();
bool isMidPressed();

#endif