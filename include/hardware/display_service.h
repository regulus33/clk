#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "development/debug_utils.h"
#include "U8x8lib.h"
#include <Wire.h>

constexpr const char *divStrings[4] = {
        "Div1 %03d",
        "Div2 %03d",
        "Div3 %03d",
        "Div4 %03d"
};

enum PrintType {
    BPM,
    Div,
    Rhythm,
};

class DisplayService {
private:
    static U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;  // Static OLED object

    static const char *getFormat(PrintType print_type, GPIOIndex index = GPIOIndex::ONE) {
        switch (print_type) {
// Think of these a bit like the arguments to strftime in ruby
            case BPM:
                return "BPM: %03d";
            case Div:
                return getDividerFormatString(index);
            case Rhythm:
                return "BIT: %03d";
            default:
                return "";
        }
    }

    static const char *getDividerFormatString(GPIOIndex index) {
        return divStrings[index];
    }

public:
    static void buildPrintString(PrintType print_type, uint8_t value, char *char_buffer, GPIOIndex index = GPIOIndex::ONE) {
        const char *format = getFormat(print_type, index);
        sprintf(char_buffer, format, value);
    }

    static void setup() {
        if (!u8x8.begin()) {
            DEBUG_PRINTLN("[OLED_DISPLAY][ERROR] - error in u8x8.begin()");
        }
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        DEBUG_PRINTLN("[OLED_DISPLAY][SETUP]");
    }

    static void printLine(uint8_t value, PrintType print_type = BPM, GPIOIndex index = GPIOIndex::ONE) {
        char char_buffer[17];
        buildPrintString(print_type, value, char_buffer, index);
        u8x8.draw2x2String(0, 3, char_buffer);
    }

//    static void printLine(char* char_buffer, PrintType _print_type = BPM) {
//        u8x8.draw2x2String(0, 3, char_buffer);
//    }
};

U8X8_SSD1306_128X64_NONAME_HW_I2C DisplayService::u8x8 = U8X8_SSD1306_128X64_NONAME_HW_I2C();  // Initialize static OLED object
#endif //OLED_DISPLAY_H
