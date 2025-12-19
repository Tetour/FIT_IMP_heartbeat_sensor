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
  
  // Prepare BPM text
  String bpmText = (bpm > 0) ? String(bpm) : "--";
  
  // Calculate text bounds for right alignment
  int16_t x, y;
  uint16_t w, h;
  display.setTextSize(3);
  display.getTextBounds(bpmText, 0, 0, &x, &y, &w, &h);
  
  // Right-align BPM (screen width 128, leave some margin)
  int rightX = 118 - w;
  display.setCursor(rightX, 25);
  display.print(bpmText);
  
  // BPM label (position relative to BPM)
  display.setTextSize(1);
  display.setCursor(rightX + w + 5, 40);
  display.println(F("BPM"));
  
  display.display();
}


void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello World"));
  display.display();
}