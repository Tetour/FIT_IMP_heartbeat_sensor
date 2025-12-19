#include "display.hpp"

Display::Display() : 
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    currentSelection(MenuOption::OFFSET),
    offsetValue(0),
    thresholdValue(50),
    peakDecayValue(25),
    troughDecayValue(10) {
}

void Display::init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  // Set default text color
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
}

void Display::showBPM(int bpm) {
  display.clearDisplay();
  
  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Heart Rate Monitor"));
  
  // BPM value on the left (right-aligned in 3-digit space)
  display.setTextSize(3);
  display.setCursor(10, 25);
  if (bpm > 0) {
    // Format to always show 3 characters, right-aligned
    char bpmStr[4];
    sprintf(bpmStr, "%3d", bpm);
    display.print(bpmStr);
  } else {
    display.print(" --");
  }
  
  // BPM label on the right
  display.setTextSize(1);
  display.setCursor(100, 40);
  display.println(F("BPM"));
  
  display.display();
}

void Display::showMenu() {
  display.clearDisplay();
  
  // Menu title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Settings"));
  
  // Menu options with values
  display.setTextSize(1);
  
  // Offset option
  display.setCursor(0, 20);
  display.printf("%sOffset:", getPrefix(MenuOption::OFFSET));
  display.setCursor(100, 20);
  display.printf("%4d", offsetValue);
  
  // Threshold option
  display.setCursor(0, 30);
  display.printf("%sThreshold:", getPrefix(MenuOption::THRESHOLD));
  display.setCursor(100, 30);
  display.printf("%4d", thresholdValue);
  
  // Peak decay rate option
  display.setCursor(0, 40);
  display.printf("%sPDR:", getPrefix(MenuOption::PEAK_DECAY));
  display.setCursor(100, 40);
  display.printf("%4d", peakDecayValue);
  
  // Through decay rate option
  display.setCursor(0, 50);
  display.printf("%sTDR:", getPrefix(MenuOption::TROUGH_DECAY));
  display.setCursor(100, 50);
  display.printf("%4d", troughDecayValue);

  display.display();
}

void Display::handleUpMovement() {
  int current = static_cast<int>(currentSelection);
  int numOptions = static_cast<int>(MenuOption::NUM_OF_OPTIONS);
  current = (current - 1 + numOptions) % numOptions;
  currentSelection = static_cast<MenuOption>(current);
}

void Display::handleDownMovement() {
  int current = static_cast<int>(currentSelection);
  int numOptions = static_cast<int>(MenuOption::NUM_OF_OPTIONS);
  current = (current + 1) % numOptions;
  currentSelection = static_cast<MenuOption>(current);
}

void Display::handleLeftMovement() {
  switch (currentSelection) {
    case MenuOption::OFFSET:
      offsetValue = max(offsetMin, offsetValue - offsetStep);
      break;
    case MenuOption::THRESHOLD:
      thresholdValue = max(thresholdMin, thresholdValue - thresholdStep);
      break;
    case MenuOption::PEAK_DECAY:
      peakDecayValue = max(peakDecayMin, peakDecayValue - peakDecayStep);
      break;
    case MenuOption::TROUGH_DECAY:
      troughDecayValue = max(troughDecayMin, troughDecayValue - troughDecayStep);
      break;
    default:
      break;
  }
}

void Display::handleRightMovement() {
  switch (currentSelection) {
    case MenuOption::OFFSET:
      offsetValue = min(offsetMax, offsetValue + offsetStep);
      break;
    case MenuOption::THRESHOLD:
      thresholdValue = min(thresholdMax, thresholdValue + thresholdStep);
      break;
    case MenuOption::PEAK_DECAY:
      peakDecayValue = min(peakDecayMax, peakDecayValue + peakDecayStep);
      break;
    case MenuOption::TROUGH_DECAY:
      troughDecayValue = min(troughDecayMax, troughDecayValue + troughDecayStep);
      break;
    default:
      break;
  }
}

const char* Display::getPrefix(MenuOption option) const {
  return (currentSelection == option) ? "> " : "  ";
}