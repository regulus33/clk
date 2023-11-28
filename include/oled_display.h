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
        oled.begin(&Adafruit128x64, I2C_ADDRESS);
        oled.setFont(System5x7);
        oled.set2X();
        oled.clear();
    }

    void printLine(int value, int text_size = TEXT_SIZE) {
        char buffer[4];  // A buffer to hold the resulting string, should be large enough to hold the number and the null terminator

        // Convert the integer to a char array (string) in decimal base
        itoa(value, buffer, 10);
        printLine(buffer);
    }


    void printLine(const char *value, int text_size = TEXT_SIZE) {
        oled.clear();
#ifdef XDEBUG
        uint8_t width = oled.strWidth(value);
        uint8_t displayWidth = oled.displayWidth();
        uint8_t horizontalMidPoint = (displayWidth - width) / 2;
        Serial.print(F("[OLED_DISPLAY][PRINT_LINE] - width of input string: "));
        Serial.print(width);
        Serial.println(F(""));
        Serial.print(F("[OLED_DISPLAY][PRINT_LINE] - width of display: "));
        Serial.print(displayWidth);
        Serial.println(F(""));
        Serial.print(F("[OLED_DISPLAY][PRINT_LINE] - horizontal midpoint: "));
        Serial.print(horizontalMidPoint);
        Serial.println(F(""));
        Serial.print(F("[OLED_DISPLAY][PRINT_LINE] - screen height: "));
        Serial.print( oled.fontRows());
        Serial.println(F(""));
#endif
        oled.setCursor(52,  3);
        oled.print(value);
    }
};


#endif //OLED_DISPLAY_H
