#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "development/debug_utils.h"
#include "U8x8lib.h"
#include <Wire.h>

// TODO: no tests!
// Static "strings" used to print to the screen info about what division the device is in
constexpr const char *divStrings[4] = {
        "Div1 %03d",
        "Div2 %03d",
        "Div3 %03d",
        "Div4 %03d"
};

// Different types of print prefixes.
enum PrintType {
    BPM,
    Div,
    Rhythm, // only in tests, not really used yet.
};

// All functions that give us the ability to interact with the SSD1306 that this project requires.
class DisplayService {
private:
    // create an instance of the OLED's driver
    static U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;  // Static OLED object
    static bool displayEnabled; // this is a user configurable setting turned on when user holds down button2 on powerup

    // based on print type and GPIO index return the template string...
    static const char *getFormat(PrintType print_type, GPIOIndex index = GPIOIndex::ONE) {
        switch (print_type) {
            // Think of these a bit like the arguments to strftime in ruby
            case BPM:
                return "BPM: %03d";
            case Div:
                // there is a better way to do this
                return getDividerFormatString(index);
            case Rhythm:
                // TODO might not need this
                return "BIT: %03d";
            default:
                return "";
        }
    }

    // returns the normal formatted string but with Div<n> prepended
    static const char *getDividerFormatString(GPIOIndex index) {
        return divStrings[index];
    }

public:
    // takes in some info and a char buffer and manipulates that char buffer based on the values.
    static void buildPrintString(PrintType print_type, uint8_t value, char *char_buffer, GPIOIndex index = GPIOIndex::ONE) {
        const char *format = getFormat(print_type, index);
        sprintf(char_buffer, format, value);
    }

    // Basic setup of hardware and ux8x8 lib *LOW MEMORY OLED LIB not super performant but eats way less mem than adafrt
    static void setup() {
        if (!u8x8.begin()) {
            DEBUG_PRINTLN("[OLED_DISPLAY][ERROR] - error in u8x8.begin()");
        }
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        DEBUG_PRINTLN("[OLED_DISPLAY][SETUP]");
    }

    // prints a few dunamic values to the screen, tightly coupled with firmware, not to be used for debugging. very
    // deterministic.
    static void updateOled(uint8_t value, PrintType print_type = BPM, GPIOIndex index = GPIOIndex::ONE) {
        if(displayEnabled) {
            char char_buffer[17];
            buildPrintString(print_type, value, char_buffer, index);
            u8x8.draw2x2String(0, 3, char_buffer);
        }

    }

    // print any "string"
    static void drawCharBuffer(char* char_buffer) {
        u8x8.draw2x2String(0, 3, char_buffer);
    }

    // disable the display (for performance, done on startup)
    static void disableDisplay() {
        displayEnabled = false;
    }

};

U8X8_SSD1306_128X64_NONAME_HW_I2C DisplayService::u8x8 = U8X8_SSD1306_128X64_NONAME_HW_I2C();  // Initialize static OLED object
bool DisplayService::displayEnabled = true;
#endif //OLED_DISPLAY_H
