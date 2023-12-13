//
// Created by zachary rowden on 07.12.2023.
//

#ifndef CLK_BUTTON_H
#define CLK_BUTTON_H

#include <Arduino.h>
#include "debug_utils.h"
#include "stdio.h"


class Button {
public:
    enum class State {
        Released,
        DebouncePress,
        Pressed,
        HeldDown,
        DebounceRelease
    };

    Button(uint8_t p) : pin(p), state(State::Released), lastDebounceTime(0) {
        pinMode(pin, INPUT);
    }


    uint8_t getPinValue() {
        switch (pin) {
            case 4:
                return (PIND & B00010000) >> 4;
            case 5:
                return (PIND & B00100000) >> 5;
            case 6:
                return (PIND & B01000000) >> 6;
            case 7:
                return (PIND & B10000000) >> 7;
        }
        DEBUG_PRINTLN("[BUTTON][PIN_VALUE][ERROR][UNMATCHED_SWATCH]");
    }

    void update() {
        uint8_t pinValue = getPinValue();
        switch (state) {
            case State::Released:
                if (pinValue == 0) {
                    state = State::DebouncePress;
                    lastDebounceTime = millis();
                    DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::DebouncePress]");
                }

                break;
            case State::DebouncePress:
                if ((millis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == 0) {
                        state = State::Pressed;
                        lastHoldTime = millis();
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
                    } else {
                        state = State::Released;
                    }
                }

                break;
            case State::Pressed:
                if (pinValue == 1) {
                    state = State::DebounceRelease;
                    lastDebounceTime = millis();
                    DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::DebounceRelease]");
                }

                break;
            case State::DebounceRelease:
                if ((millis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == 1) {
                        state = State::Released;
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Released]");
                    } else {
                        state = State::Pressed;
                        lastHoldTime = millis();
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
                    }
                }

                break;
        }
    }

    bool isPressed() const {
        return state == State::Pressed;
    };

    void setupInterrupt(void (*ISR)(void), int mode) const {
        attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
    }


private:
    uint8_t pin;
    volatile State state = State::Released;
    unsigned long lastDebounceTime;
    unsigned long lastHoldTime{};
    const unsigned long debounceDelay = 50; // debounce time in milliseconds
    const unsigned long holdDelay = 1000;

    static void handleInterrupt() {
        // This method will be called on interrupt
        // TODO: Update state or set flags as necessary
    }

    char* printState(State state) {
        char *stateString;
        switch (state) {
            case State::DebouncePress:
                stateString = "DEBOUNCE_PRESS";
                break;
            case State::Pressed:
                stateString = "PRESSED";
                break;
            case State::DebounceRelease:
                stateString = "DEBOUNCE_RELEASE";
                break;
            case State::Released:
                stateString = "RELEASED";
                break;
        }
        return stateString;
    }

//    TODO:
//    void readButtonPulseAndUpdateDivisions(Division& division, int buttonStatesIndex, uint8_t currentState) {
//        //TODO: what benefit are we gaining from currentState != buttonStates[buttonStatesIndex] if we alsio do the currentState == 0 check below
//        if (currentState != buttonStates[buttonStatesIndex]) {
//            buttonStates[buttonStatesIndex] = currentState;
//            unsigned long currentTime = millis();
//            //TODO: or maybe a better question, why do we need to ask currentState == 0 if we already know that the state has changed? i.e. currentState != buttonStates[buttonStatesIndex] this feels like a bandaid
//            if (currentTime - buttonLastChanged[buttonStatesIndex] > DEBOUNCE_DELAY && currentState == 0) {
//                oledDisplay.printLine(division.incrementDiv(), Div);
//                buttonLastChanged[buttonStatesIndex] = currentTime;
//            }
//
//        }
//    }

};

#endif //CLK_BUTTON_H
