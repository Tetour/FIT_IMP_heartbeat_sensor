#include "display.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  // Set default text color
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
}

void displayBPM(int bpm) {
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

void displayMenu() {
  display.clearDisplay();
  
  // Menu title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Settings"));
  
  // Menu options
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println(F("Offset"));
  display.setCursor(10, 30);
  display.println(F("Threshold"));
  display.setCursor(10, 40);
  display.println(F("Peak decay rate"));
  display.setCursor(10, 50);
  display.println(F("Through decay rate"));

  display.display();
}