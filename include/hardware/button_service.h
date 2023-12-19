//
// Created by zachary rowden on 07.12.2023.
//

#ifndef CLK_BUTTON_SERVICE_H
#define CLK_BUTTON_SERVICE_H
#define BUTTON_COUNT 4

#include <Arduino.h>
#include "development/debug_utils.h"
#include "stdio.h"


class ButtonService {
public:
    enum class State {
        Released,
        DebouncePress,
        Pressed,
        HeldDown,
        DebounceRelease
    };

    ButtonService(uint8_t p) : pin(p), state(State::Released), lastDebounceTime(0) {}

    void setup() {
        pinMode(pin, INPUT_PULLUP);
        DEBUG_PRINT("[BUTTON][");
        DEBUG_PRINT_VAR(pin);
        DEBUG_PRINTLN("][SETUP]");
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

    }

private:
    uint8_t pin;
    volatile State state = State::Released;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = 50;
//    unsigned long lastHoldTime{};
//    const unsigned long holdDelay = 1000;


    char *printState(State state) {
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
};

#endif //CLK_BUTTON_SERVICE_H
