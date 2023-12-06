//
// Created by zack on 9/3/23.
//
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <U8x8lib.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define I2C_ADDRESS 0x3C

enum PrintType {
    BPM,
    Div,
    Rhythm,
};

class OledDisplay {
private:
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

public:
    void setup() {
        u8x8.begin();
        u8x8.setFont(u8x8_font_chroma48medium8_r);

    }

    /* convert int to char buffer and feed to char buffer implementation of print */
    void printLine(uint8_t value, PrintType printType = BPM) {
        char charBuffer[17];

        switch (printType) {
            case BPM:
                sprintf(charBuffer, "BPM: %03d", value);
                break;
            case Div:
                sprintf(charBuffer, "DIV: %03d", value);
                break;
            case Rhythm:
                sprintf(charBuffer, "BIT: %03d", value);
                break;
        }

        printLine(charBuffer, printType);
    }

    void printLine(char* charBuffer, PrintType printType = BPM) {
        u8x8.draw2x2String(0,3,charBuffer);
    }
};

#endif //OLED_DISPLAY_H
