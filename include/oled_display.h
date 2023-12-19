#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H
#include "debug_utils.h"
#include <U8x8lib.h>
#include <Wire.h>

enum PrintType {
    BPM,
    Div,
    Rhythm,
};

class OledDisplay {
private:
    static U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;  // Static OLED object

    static const char* getFormat(PrintType print_type) {
        switch (print_type) {
            case BPM: return "BPM: %03d";
            case Div: return "DIV: %03d";
            case Rhythm: return "BIT: %03d";
            default: return "";
        }
    }

public:
    static void buildPrintString(PrintType print_type, uint8_t value, char* char_buffer) {
        const char* format = getFormat(print_type);
        sprintf(char_buffer, format, value);
    }

    static void setup() {
        if (!u8x8.begin()) {
            DEBUG_PRINTLN("[OLED_DISPLAY][ERROR] - error in u8x8.begin()");
        }
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        DEBUG_PRINTLN("[OLED_DISPLAY][SETUP]");
    }

    static void printLine(uint8_t value, PrintType print_type = BPM) {
        char char_buffer[17];
        buildPrintString(print_type, value, char_buffer);
        u8x8.draw2x2String(0, 3, char_buffer);
    }

//    static void printLine(char* char_buffer, PrintType _print_type = BPM) {
//        u8x8.draw2x2String(0, 3, char_buffer);
//    }
};

U8X8_SSD1306_128X64_NONAME_HW_I2C OledDisplay::u8x8 = U8X8_SSD1306_128X64_NONAME_HW_I2C();  // Initialize static OLED object
#endif //OLED_DISPLAY_H
