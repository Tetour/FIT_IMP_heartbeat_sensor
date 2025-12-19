#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class Display {
private:
    Adafruit_SSD1306 display;
    
public:
    Display();
    void init();
    void showBPM(int bpm);
    void showMenu();
};
