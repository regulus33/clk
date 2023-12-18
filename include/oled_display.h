#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H
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

public:
    static void setup() {
        u8x8.begin();
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        DEBUG_PRINTLN("[OLED_DISPLAY][SETUP]");
    }

    static void printLine(uint8_t value, PrintType print_type = BPM) {
        char char_buffer[17];

        switch (print_type) {
            case BPM:
                sprintf(char_buffer, "BPM: %03d", value);
                break;
            case Div:
                sprintf(char_buffer, "DIV: %03d", value);
                break;
            case Rhythm:
                sprintf(char_buffer, "BIT: %03d", value);
                break;
        }

        printLine(char_buffer, print_type);

    }

    static void printLine(char* char_buffer, PrintType print_type = BPM) {
        u8x8.draw2x2String(0, 3, char_buffer);
    }
};

U8X8_SSD1306_128X64_NONAME_HW_I2C OledDisplay::u8x8 = U8X8_SSD1306_128X64_NONAME_HW_I2C();  // Initialize static OLED object
#endif //OLED_DISPLAY_H
