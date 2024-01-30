//
// Created by zachary rowden on 19.12.2023.
//
#ifndef CLK_BUTTON_STATE_H
#define CLK_BUTTON_STATE_H

#include "Arduino.h"
#include "development/debug_utils.h"
#include "gpio_index.h"
#include "clock_mode.h"
#include "division_mode.h"

// pressed state
constexpr uint8_t PRESSED = 0;
constexpr uint8_t RELEASED = 1;
constexpr unsigned long DEBOUNCE_DELAY = 50;
constexpr unsigned long HOLD_DELAY = 1000;

typedef void (*DivisionModeChangeCallback)(DivisionMode, GPIOIndex);

typedef void (*DivisionChangeCallback)(GPIOIndex, uint8_t);

typedef void (*ClockModeChangeCallback)(ClockMode);

struct ButtonState {
    // physical button mapping
    GPIOIndex ioIndex;
    // states
    enum class State { Released, DebouncePress, Pressed, HeldDown, DebounceRelease };
    // initial state
    State state = State::Released;

    // when we "debounce" the goal is to make sure that when a state is entered it more or less stays that way.
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = DEBOUNCE_DELAY;
    unsigned long lastHoldTime{};
    const unsigned long holdDelay = HOLD_DELAY;
    uint8_t heldDownWasPressed = false;

//    uint8_t startupFlagFlipped = false;

    DivisionModeChangeCallback divisionModeChangeCallback = nullptr;
    DivisionChangeCallback divisionChangeCallback = nullptr;
    ClockModeChangeCallback clockModeChangeCallback = nullptr;
    // Held down, operation cancelled 🚫👇
    // 🤡 mock millis() so we can test state machine
#ifdef TEST_BUILD
    unsigned long mockMillis = 0;
#endif

    unsigned long mMillis() {
#ifdef TEST_BUILD
        return mockMillis;
#else
        return millis();
#endif
    }

    void getNextStateFromReleased(uint8_t pinValue) {
        if (pinValue == PRESSED) {
            state = State::DebouncePress;
            lastDebounceTime = mMillis();
            DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::DebouncePress]");
        }
    }

    void getNextStateFromDebouncePressed(uint8_t pinValue) {
        if ((mMillis() - lastDebounceTime) > debounceDelay) {
            if (pinValue == PRESSED) {
                state = State::Pressed;
                heldDownWasPressed = false;
                lastHoldTime = mMillis();
                divisionChangeCallback(ioIndex, false);
                DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
            } else {
                // this transition does not represent an action, it is just to return to default nothing state
                state = State::Released;
            }
        }
    }

    void getNextStateFromPressed(uint8_t pinValue) {
        if (pinValue == RELEASED) {
            state = State::DebounceRelease;
            lastDebounceTime = mMillis();
            DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::DebounceRelease]");
        } else {
            if (mMillis() - lastHoldTime > holdDelay) {
                state = State::HeldDown;
                heldDownWasPressed = true;
                DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::HeldDown]");
            }
        }
    }

    void getNextStateFromDebounceRelease(uint8_t pinValue) {
        if ((mMillis() - lastDebounceTime) > debounceDelay) {
            if (pinValue == RELEASED) {
                state = State::Released;
                DEBUG_PRINT("[CALLBACK][ABOUT_TO_CALL]divisionChangeCallback] - ioIndex");
                DEBUG_PRINTLN_VAR(ioIndex);
                if(!heldDownWasPressed) {
                    divisionChangeCallback(ioIndex, true);
                } else {
                    DEBUG_PRINT("[BUTTON][STATE_CHANGE][HELD_DOWN_WAS_PRESSED] - SKIPPING ACTION!");
                }

                DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Released]");
            } else {
                state = State::Pressed;
                heldDownWasPressed = false;
                lastHoldTime = mMillis();
                DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
            }
        }
    }

    void getNextStateFromHeldDown(uint8_t pinValue) {
        if (pinValue == RELEASED) {
            state = State::DebounceRelease;
            lastDebounceTime = mMillis();
            DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::DebounceRelease]");
//TODO
//                    if (!startupFlagFlipped && ioIndex == GPIOIndex::ONE) {
//                        clockModeChangeCallback(ClockMode::External);
//                    }
        }
    }

    ButtonState(GPIOIndex ioIndex) : ioIndex(ioIndex) {}

    void updateState(uint8_t pinValue) {
        switch (state) {
            case State::Released:
                getNextStateFromReleased(pinValue);
                break;
            case State::DebouncePress:
                getNextStateFromDebouncePressed(pinValue);
                break;
            case State::Pressed:
                getNextStateFromPressed(pinValue);
                break;
            case State::HeldDown:
                getNextStateFromHeldDown(pinValue);
                break;
            case State::DebounceRelease:
                getNextStateFromDebounceRelease(pinValue);
                break;
        }
    }
};

#endif //CLK_BUTTON_STATE_H
