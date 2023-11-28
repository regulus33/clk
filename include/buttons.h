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
    uint8_t buttonStates[4] = {1, 1, 1, 1};
    unsigned long buttonLastChanged[4] = {0, 0, 0, 0};
    OledDisplay& oledDisplay;
    ClockManager& clockManager;

    // Constructor to set pin modes
    Buttons(OledDisplay& oledDisplay, ClockManager& clockManager) : oledDisplay(oledDisplay), clockManager(clockManager) {
        pinMode(7, INPUT_PULLUP);
        pinMode(6, INPUT_PULLUP);
        pinMode(5, INPUT_PULLUP);
        pinMode(4, INPUT_PULLUP);
    }


    void readButtonPulseAndUpdateDivisions(Division& division, int buttonStatesIndex, uint8_t currentState) {
        //TODO: what benefit are we gaining from currentState != buttonStates[buttonStatesIndex] if we alsio do the currentState == 0 check below
        if (currentState != buttonStates[buttonStatesIndex]) {
            buttonStates[buttonStatesIndex] = currentState;
            unsigned long currentTime = millis();
            //TODO: or maybe a better question, why do we need to ask currentState == 0 if we already know that the state has changed? i.e. currentState != buttonStates[buttonStatesIndex] this feels like a bandaid
            if (currentTime - buttonLastChanged[buttonStatesIndex] > DEBOUNCE_DELAY && currentState == 0) {
                oledDisplay.printLine(division.incrementDiv(), 1);
                buttonLastChanged[buttonStatesIndex] = currentTime;
            }

        }
    }

    // Read current states of buttons
    void read() {
        readButtonPulseAndUpdateDivisions(clockManager.division1, 0, (PIND & B10000000) >> 7);
        readButtonPulseAndUpdateDivisions(clockManager.division2, 1, (PIND & B01000000) >> 6);
        readButtonPulseAndUpdateDivisions(clockManager.division3, 2, (PIND & B00100000) >> 5);
        readButtonPulseAndUpdateDivisions(clockManager.division4, 3, (PIND & B00010000) >> 4);
    }
};

#endif // BUTTONS_H
#endif //CLK_BUTTONS_H
