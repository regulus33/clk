//
// Created by zack on 10/8/23.
//

#ifndef CLK_BUTTONS_H
#define CLK_BUTTONS_H
#ifndef BUTTONS_H
#define BUTONS_H

#include <Arduino.h>
#include "oled_display.h"
#include "clock_manager.h"

#define DEBOUNCE_DELAY 50

class Buttons {
public:
    // Initialize arrays to store button states, pressed states and last changed timestamps
    uint8_t buttonStates[4] = {0, 0, 0, 0};
    uint8_t buttonPressedStates[4] = {0, 0, 0, 0};
    unsigned long buttonLastChanged[4] = {0, 0, 0, 0};
    OledDisplay& oledDisplay;
    ClockManager& clockManager;

    // Constructor to set pin modes
    Buttons(OledDisplay& oledDisplay, ClockManager& clockManager) : oledDisplay(oledDisplay), clockManager(clockManager) {
        pinMode(7, INPUT_PULLUP);
        pinMode(6, INPUT_PULLUP);
        pinMode(5, INPUT_PULLUP);
        pinMode(12, INPUT_PULLUP);
    }

    // Read current states of buttons
    void read() {

        uint8_t currentState = (PIND & B10000000) >> 7;
        if (currentState != buttonStates[0]) {
            buttonStates[0] = currentState;
            unsigned long currentTime = millis();
            if (currentTime - buttonLastChanged[0] > DEBOUNCE_DELAY && currentState == 0) {
                    oledDisplay.printLine(clockManager.division1.incrementDiv(), 1);
            }
            buttonLastChanged[0] = currentTime;
        }


        currentState = (PIND & B01000000) >> 6;
        if (currentState != buttonStates[1]) {
            buttonStates[1] = currentState;
            unsigned long currentTime = millis();
            if (currentTime - buttonLastChanged[1] > DEBOUNCE_DELAY && currentState == 0) {
                oledDisplay.printLine(clockManager.division2.incrementDiv(), 1);
            }
            buttonLastChanged[1] = currentTime;
        }

        // Read D5 directly from the register and update state
        currentState = (PIND & B00100000) >> 5;
        if (currentState != buttonStates[2]) {
            buttonStates[2] = currentState;
            unsigned long currentTime = millis();
            if (currentTime - buttonLastChanged[2] > DEBOUNCE_DELAY && currentState == 0) {
                oledDisplay.printLine(clockManager.division3.incrementDiv(), 1);
            }
            buttonLastChanged[2] = currentTime;
        }

        // Read D12 directly from the register and update state
        currentState = (PINB & B00010000) >> 4;
        if (currentState != buttonStates[3]) {
            buttonStates[3] = currentState;
            unsigned long currentTime = millis();
            if (currentTime - buttonLastChanged[3] > DEBOUNCE_DELAY && currentState == 0) {
                oledDisplay.printLine(clockManager.division4.incrementDiv(), 1);
            }
            buttonLastChanged[3] = currentTime;
        }
    }
};

#endif // BUTTONS_H
#endif //CLK_BUTTONS_H
