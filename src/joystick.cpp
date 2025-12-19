#include "joystick.hpp"

Joystick::Joystick() :
    lastUpState(false),
    lastDownState(false),
    lastLeftState(false),
    lastRightState(false),
    lastMidState(false),
    upPressed(false),
    downPressed(false),
    leftPressed(false),
    rightPressed(false),
    midPressed(false),
    upWasPressed(false),
    downWasPressed(false),
    leftWasPressed(false),
    rightWasPressed(false),
    midWasPressed(false),
    lastUpDebounce(0),
    lastDownDebounce(0),
    lastLeftDebounce(0),
    lastRightDebounce(0),
    lastMidDebounce(0) {
}

void Joystick::init() {
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_MID, INPUT_PULLUP);
}

void Joystick::update() {
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
      upWasPressed = upPressed;  // Set edge detection flag
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
      downWasPressed = downPressed;
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
      leftWasPressed = leftPressed;
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
      rightWasPressed = rightPressed;
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
      midWasPressed = midPressed;
      if (midPressed && Serial) {
        Serial.println("MID pressed");
      }
    }
  }
  lastMidState = midReading;
}

bool Joystick::isUpPressed() {
  return upPressed;
}

bool Joystick::isDownPressed() {
  return downPressed;
}

bool Joystick::isLeftPressed() {
  return leftPressed;
}

bool Joystick::isRightPressed() {
  return rightPressed;
}

bool Joystick::isMidPressed() {
  return midPressed;
}

bool Joystick::wasUpPressed() {
  bool result = upWasPressed;
  upWasPressed = false;
  return result;
}

bool Joystick::wasDownPressed() {
  bool result = downWasPressed;
  downWasPressed = false;
  return result;
}

bool Joystick::wasLeftPressed() {
  bool result = leftWasPressed;
  leftWasPressed = false;
  return result;
}

bool Joystick::wasRightPressed() {
  bool result = rightWasPressed;
  rightWasPressed = false;
  return result;
}

bool Joystick::wasMidPressed() {
  bool result = midWasPressed;
  midWasPressed = false;
  return result;
}