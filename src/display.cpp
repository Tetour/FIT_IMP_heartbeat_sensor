#include "display.hpp"

Display::Display(Sensor& sensorRef, DataLogger& loggerRef) : 
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    currentSelection(MenuOption::DATA_RECORDING),
    sensor(sensorRef),
    dataLogger(loggerRef),
    debugOutput(false),
    signalHistoryIndex(0) {
  // Initialize signal history with zeros
  memset(signalHistory, 0, sizeof(signalHistory));
}

void Display::init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if (debugOutput && Serial) {
      Serial.println(F("SSD1306 allocation failed"));
    }
    for (;;);
  }
  
  // Set default text color
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
}

void Display::updateSignalHistory(int signalValue) {
  signalHistory[signalHistoryIndex] = signalValue;
  signalHistoryIndex = (signalHistoryIndex + 1) % SIGNAL_HISTORY_SIZE;
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
  
  // Flashing recording indicator in top right corner
  drawRecordingIndicator();
  
  display.display();
}

void Display::showSignalGraph() {
  display.clearDisplay();
  
  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Heart Signal Graph"));
  
  // Draw the signal graph
  int graphHeight = 32;  // Reduced to leave space for signal value at bottom
  int graphY = 20;
  int graphWidth = 128;  // Full screen width
  
  // Find min and max values in the signal history for scaling
  int minVal = 1023;
  int maxVal = 0;
  for (int i = 0; i < SIGNAL_HISTORY_SIZE; i++) {
    if (signalHistory[i] < minVal) minVal = signalHistory[i];
    if (signalHistory[i] > maxVal) maxVal = signalHistory[i];
  }
  
  // Avoid division by zero
  if (maxVal == minVal) {
    maxVal = minVal + 1;
  }
  
  // Draw the graph
  for (int x = 0; x < graphWidth; x++) {
    // Calculate history index: x=0 is oldest, x=127 is newest
    int historyIndex = (signalHistoryIndex - graphWidth + x + SIGNAL_HISTORY_SIZE) % SIGNAL_HISTORY_SIZE;
    int signalValue = signalHistory[historyIndex];
    
    // Scale the signal value to fit in the graph height
    int y = graphY + graphHeight - ((signalValue - minVal) * graphHeight / (maxVal - minVal));
    
    // Draw a point
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  
  // Display current signal value at bottom
  int currentSignal = sensor.getSignal();
  
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print(F("Signal: "));
  display.print(currentSignal);
  
  // Flashing recording indicator in top right corner
  drawRecordingIndicator();
  
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
  display.printf("%4s", dataLogger.isRecording() ? "ON" : "OFF");
  
  // Autorecording option
  display.setCursor(0, 32);
  display.printf("%sAuto record:", getPrefix(MenuOption::AUTORECORDING));
  display.setCursor(100, 32);
  int recTime = dataLogger.getAutoRecordingTime();
  if (recTime == 0) {
    display.printf("%4s", "NO");
  } else {
    display.printf("%3ds", recTime);
  }
  
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
      if (dataLogger.isRecording()) {
        dataLogger.stopRecording();
      } else {
        dataLogger.startRecording("/sensor_data.csv");
      }
      break;
    }
    case MenuOption::AUTORECORDING: {
      int currentValue = dataLogger.getAutoRecordingTime();
      dataLogger.setAutoRecordingTime(max(DataLogger::getAutoRecordingMin(), currentValue - recordingStep));
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
      if (dataLogger.isRecording()) {
        dataLogger.stopRecording();
      } else {
        dataLogger.startRecording("/sensor_data.csv");
      }
      break;
    }
    case MenuOption::AUTORECORDING: {
      int currentValue = dataLogger.getAutoRecordingTime();
      dataLogger.setAutoRecordingTime(min(DataLogger::getAutoRecordingMax(), currentValue + recordingStep));
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

// Helper method for recording indicator
void Display::drawRecordingIndicator() {
  if (dataLogger.isRecording()) {
    unsigned long currentMillis = millis();
    if ((currentMillis / 500) % 2 == 0) {
      display.fillCircle(120, 5, 2, SSD1306_WHITE);
    }
  }
}

// Debug output control
void Display::setDebugOutput(bool enable) {
  debugOutput = enable;
}

bool Display::getDebugOutput() const {
  return debugOutput;
}