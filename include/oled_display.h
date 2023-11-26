//
// Created by zack on 9/3/23.
//

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEXT_SIZE 3
#define I2C_ADDRESS 0x3C
#define RST_PIN -1

class OledDisplay {
private:
    SSD1306AsciiAvrI2c oled;
public:
    void setup() {
#if RST_PIN >= 0
        oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
        oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
        // Call oled.setI2cClock(frequency) to change from the default frequency.

        oled.setFont(System5x7);
        oled.clear();
    }

    void printLine(int value, int text_size = TEXT_SIZE) {
        clear();
        oled.print(value);
    }


    void printLine(const char *value, int text_size = TEXT_SIZE) {
        clear();
        oled.print(value);
    }


    void clear() {
        oled.clear();
    }
};


#endif //OLED_DISPLAY_H
