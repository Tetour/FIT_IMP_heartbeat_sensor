#include "display.hpp"

Display::Display(Sensor& sensorRef) : 
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    currentSelection(MenuOption::DATA_RECORDING),
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
  
  // Data recording option (first item)
  display.setCursor(0, 20);
  display.printf("%sRecording:", getPrefix(MenuOption::DATA_RECORDING));
  display.setCursor(100, 20);
  display.printf("%4s", sensor.isRecording() ? "ON" : "OFF");
  
  // Offset option
  display.setCursor(0, 32);
  display.printf("%sBPM offset:", getPrefix(MenuOption::BPM_OFFSET));
  display.setCursor(100, 32);
  display.printf("%4d", sensor.getBpmOffset());
  
  // Threshold option
  display.setCursor(0, 44);
  display.printf("%sThrs offset:", getPrefix(MenuOption::BEAT_THRESHOLD));
  display.setCursor(100, 44);
  display.printf("%4d", sensor.getThresholdOffset());
  
  // Decay rate option
  display.setCursor(0, 56);
  display.printf("%sDecay rate:", getPrefix(MenuOption::DECAY_RATE));
  display.setCursor(100, 56);
  display.printf("%4d", sensor.getDecayRate());

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
    case MenuOption::DATA_RECORDING: {
      // Toggle recording on/off
      if (sensor.isRecording()) {
        sensor.stopRecording();
      } else {
        sensor.startRecording("/sensor_data.csv");
      }
      break;
    }
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
    case MenuOption::DECAY_RATE: {
      int currentValue = sensor.getDecayRate();
      sensor.setDecayRate(max(Sensor::getPeakDecayMin(), currentValue - decayStep));
      break;
    }
    default:
      break;
  }
}

void Display::handleRightMovement() {
  switch (currentSelection) {
    case MenuOption::DATA_RECORDING: {
      // Toggle recording on/off
      if (sensor.isRecording()) {
        sensor.stopRecording();
      } else {
        sensor.startRecording("/sensor_data.csv");
      }
      break;
    }
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
    case MenuOption::DECAY_RATE: {
      int currentValue = sensor.getDecayRate();
      sensor.setDecayRate(min(Sensor::getPeakDecayMax(), currentValue + decayStep));
      break;
    }
    default:
      break;
  }
}

const char* Display::getPrefix(MenuOption option) const {
  return (currentSelection == option) ? "> " : "  ";
}