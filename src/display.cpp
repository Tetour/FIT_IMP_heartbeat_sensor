#include "display.hpp"

Display::Display(Sensor& sensorRef) : 
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    currentSelection(MenuOption::BPM_OFFSET),
    sensor(sensorRef) {
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
  
  // BPM value on the left
  display.setTextSize(3);
  display.setCursor(10, 25);
  if (bpm > 0) {
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
  
  // Show BPM in top right corner
  int currentBPM = sensor.getBPM();
  display.setCursor(80, 0);
  if (currentBPM > 0) {
    display.printf("%3d", currentBPM);
  } else {
    display.print(" --");
  }
  display.setCursor(110, 0);
  display.print(F("BPM"));

  // Menu options with values
  display.setTextSize(1);
  
  // Offset option
  display.setCursor(0, 20);
  display.printf("%sBPM offset:", getPrefix(MenuOption::BPM_OFFSET));
  display.setCursor(100, 20);
  display.printf("%4d", sensor.getBpmOffset());
  
  // Threshold option
  display.setCursor(0, 30);
  display.printf("%sThrs offset:", getPrefix(MenuOption::BEAT_THRESHOLD));
  display.setCursor(100, 30);
  display.printf("%4d", sensor.getThresholdOffset());
  
  // Peak decay rate option
  display.setCursor(0, 40);
  display.printf("%sPeak DR:", getPrefix(MenuOption::PEAK_DECAY));
  display.setCursor(100, 40);
  display.printf("%4d", sensor.getPeakDecayRate());
  
  // Through decay rate option
  display.setCursor(0, 50);
  display.printf("%sTrough DR:", getPrefix(MenuOption::TROUGH_DECAY));
  display.setCursor(100, 50);
  display.printf("%4d", sensor.getTroughDecayRate());

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
    case MenuOption::BPM_OFFSET: {
      int currentValue = sensor.getBpmOffset();
      sensor.setBpmOffset(max(Sensor::getBpmOffsetMin(), currentValue - offsetStep));
      break;
    }
    case MenuOption::BEAT_THRESHOLD: {
      int currentValue = sensor.getThresholdOffset();
      sensor.setThresholdOffset(max(Sensor::getThresholdOffsetMin(), currentValue - thresholdStep));
      break;
    }
    case MenuOption::PEAK_DECAY: {
      int currentValue = sensor.getPeakDecayRate();
      sensor.setPeakDecayRate(max(Sensor::getPeakDecayMin(), currentValue - peakDecayStep));
      break;
    }
    case MenuOption::TROUGH_DECAY: {
      int currentValue = sensor.getTroughDecayRate();
      sensor.setTroughDecayRate(max(Sensor::getTroughDecayMin(), currentValue - troughDecayStep));
      break;
    }
    default:
      break;
  }
}

void Display::handleRightMovement() {
  switch (currentSelection) {
    case MenuOption::BPM_OFFSET: {
      int currentValue = sensor.getBpmOffset();
      sensor.setBpmOffset(min(Sensor::getBpmOffsetMax(), currentValue + offsetStep));
      break;
    }
    case MenuOption::BEAT_THRESHOLD: {
      int currentValue = sensor.getThresholdOffset();
      sensor.setThresholdOffset(min(Sensor::getThresholdOffsetMax(), currentValue + thresholdStep));
      break;
    }
    case MenuOption::PEAK_DECAY: {
      int currentValue = sensor.getPeakDecayRate();
      sensor.setPeakDecayRate(min(Sensor::getPeakDecayMax(), currentValue + peakDecayStep));
      break;
    }
    case MenuOption::TROUGH_DECAY: {
      int currentValue = sensor.getTroughDecayRate();
      sensor.setTroughDecayRate(min(Sensor::getTroughDecayMax(), currentValue + troughDecayStep));
      break;
    }
    default:
      break;
  }
}

const char* Display::getPrefix(MenuOption option) const {
  return (currentSelection == option) ? "> " : "  ";
}