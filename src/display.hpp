#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

extern Adafruit_SSD1306 display;

void initDisplay();
void displayBPM(int bpm);
void displayMenu();

#endif