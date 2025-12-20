#include "joystick.hpp"

Joystick::Joystick() :
    JOY_UP_PIN(18),
    JOY_DOWN_PIN(19),
    JOY_LEFT_PIN(23),
    JOY_RIGHT_PIN(5),
    JOY_MID_PIN(13),
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
    lastMidDebounce(0),
    debugOutput(false),
    DEBOUNCE_DELAY(50) {
}

void Joystick::init() {
  pinMode(JOY_UP_PIN, INPUT_PULLUP);
  pinMode(JOY_DOWN_PIN, INPUT_PULLUP);
  pinMode(JOY_LEFT_PIN, INPUT_PULLUP);
  pinMode(JOY_RIGHT_PIN, INPUT_PULLUP);
  pinMode(JOY_MID_PIN, INPUT_PULLUP);
}

void Joystick::update() {
  unsigned long now = millis();

  // Read current states
  bool upReading = digitalRead(JOY_UP_PIN) == LOW;
  bool downReading = digitalRead(JOY_DOWN_PIN) == LOW;
  bool leftReading = digitalRead(JOY_LEFT_PIN) == LOW;
  bool rightReading = digitalRead(JOY_RIGHT_PIN) == LOW;
  bool midReading = digitalRead(JOY_MID_PIN) == LOW;

  // Debounce and detect edges for UP
  if (upReading != lastUpState) {
    lastUpDebounce = now;
  }
  if ((now - lastUpDebounce) > DEBOUNCE_DELAY) {
    if (upReading != upPressed) {
      upPressed = upReading;
      upWasPressed = upPressed;  // Set edge detection flag
      if (upPressed && debugOutput && Serial) {
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
      if (downPressed && debugOutput && Serial) {
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
      if (leftPressed && debugOutput && Serial) {
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
      if (rightPressed && debugOutput && Serial) {
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
      if (midPressed && debugOutput && Serial) {
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

// Debug output control
bool Joystick::getDebugOutput() const {
  return debugOutput;
}

void Joystick::setDebugOutput(bool enable) {
  debugOutput = enable;
}