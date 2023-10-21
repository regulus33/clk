//
// Created by zack on 9/3/23.
//

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Arduino.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEXT_SIZE 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 d(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class OledDisplay {
private:
    Adafruit_SSD1306& display = d;
public:
    void setup() {
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            for(;;);
        }
        clear();
        display.display();
    }

    void printLine(int value, int text_size = TEXT_SIZE) {
        char buffer[4];  // make sure this is large enough to hold all digits and null terminator;
        itoa(value, buffer, 10);
        printLine(buffer);
    }


    void printLine(const char* value, int text_size = TEXT_SIZE){
        clear();
        uint16_t width = SCREEN_WIDTH;
        uint16_t height = SCREEN_HEIGHT;

        uint16_t w, h;

        display.getTextBounds(value, 0, 0, nullptr, nullptr, &w, &h); // Calculate the bounds of the text

        /*! Calculate the position to start the text to make it centered */
        int x = (width - w) / 2;
        int y = (height - h) / 2;

        /*! Clear the display, set the cursor position, and then print the text */
        display.setTextSize(text_size); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.clearDisplay();
        display.setCursor(x, y);
        display.print(value);
        display.display();
    }

    void hardwareTest() {
        clear();
        printLine("Clawk");
        delay(1);
        drawSplashCircles();
        printLine(255);
    }

    void clear() {
        display.clearDisplay();
    }

private:
    void drawSplashCircles(void) {
        display.clearDisplay();
        for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
            // The INVERSE color is used so circles alternate white/black
            display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
            display.display(); // Update screen with each newly-drawn circle
            delay(0.2);
        }
    }
};


#endif //OLED_DISPLAY_H
