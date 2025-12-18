#include "joystick.hpp"

// State tracking for edge detection
static bool lastUpState = false;
static bool lastDownState = false;
static bool lastLeftState = false;
static bool lastRightState = false;
static bool lastMidState = false;

// Current debounced states
static bool upPressed = false;
static bool downPressed = false;
static bool leftPressed = false;
static bool rightPressed = false;
static bool midPressed = false;

// Last debounce times
static unsigned long lastUpDebounce = 0;
static unsigned long lastDownDebounce = 0;
static unsigned long lastLeftDebounce = 0;
static unsigned long lastRightDebounce = 0;
static unsigned long lastMidDebounce = 0;

void initJoystick() {
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_MID, INPUT_PULLUP);
}

void updateJoystick() {
  unsigned long now = millis();
  
  // Read current states
  bool upReading = digitalRead(JOY_UP) == LOW;
  bool downReading = digitalRead(JOY_DOWN) == LOW;
  bool leftReading = digitalRead(JOY_LEFT) == LOW;
  bool rightReading = digitalRead(JOY_RIGHT) == LOW;
  bool midReading = digitalRead(JOY_MID) == LOW;
  
  // Debounce and detect edges for UP
  if (upReading != lastUpState) {
    lastUpDebounce = now;
  }
  if ((now - lastUpDebounce) > DEBOUNCE_DELAY) {
    if (upReading != upPressed) {
      upPressed = upReading;
      if (upPressed && Serial) {
        Serial.println("UP pressed");
      }
    }
  }
  lastUpState = upReading;
  
  // Debounce and detect edges for DOWN
  if (downReading != lastDownState) {
    lastDownDebounce = now;
  }
  if ((now - lastDownDebounce) > DEBOUNCE_DELAY) {
    if (downReading != downPressed) {
      downPressed = downReading;
      if (downPressed && Serial) {
        Serial.println("DOWN pressed");
      }
    }
  }
  lastDownState = downReading;
  
  // Debounce and detect edges for LEFT
  if (leftReading != lastLeftState) {
    lastLeftDebounce = now;
  }
  if ((now - lastLeftDebounce) > DEBOUNCE_DELAY) {
    if (leftReading != leftPressed) {
      leftPressed = leftReading;
      if (leftPressed && Serial) {
        Serial.println("LEFT pressed");
      }
    }
  }
  lastLeftState = leftReading;
  
  // Debounce and detect edges for RIGHT
  if (rightReading != lastRightState) {
    lastRightDebounce = now;
  }
  if ((now - lastRightDebounce) > DEBOUNCE_DELAY) {
    if (rightReading != rightPressed) {
      rightPressed = rightReading;
      if (rightPressed && Serial) {
        Serial.println("RIGHT pressed");
      }
    }
  }
  lastRightState = rightReading;
  
  // Debounce and detect edges for MID
  if (midReading != lastMidState) {
    lastMidDebounce = now;
  }
  if ((now - lastMidDebounce) > DEBOUNCE_DELAY) {
    if (midReading != midPressed) {
      midPressed = midReading;
      if (midPressed && Serial) {
        Serial.println("MID pressed");
      }
    }
  }
  lastMidState = midReading;
}

bool isUpPressed() {
  return upPressed;
}

bool isDownPressed() {
  return downPressed;
}

bool isLeftPressed() {
  return leftPressed;
}

bool isRightPressed() {
  return rightPressed;
}

bool isMidPressed() {
  return midPressed;
}