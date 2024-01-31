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
#include "config.h"
#include "callbacks.h"

// pressed state. 0 is pushed down (broken circuit)
constexpr uint8_t PRESSED = 0;
constexpr uint8_t RELEASED = 1;

// Button state, it is supposed to just be state it is more than that bc of the state machine...
struct ButtonState {
    // physical button mapping
    GPIOIndex ioIndex;
    // states for state machine
    enum class State {
        Released, DebouncePress, Pressed, HeldDown, DebounceRelease
    };
    // initial state
    State state = State::Released;

    // track last debounce time, so we can substract it from millis()
    unsigned long lastStateChangeTimeMillis;
    // same as lastStateChangeTimeMillis except applies to HeldDown instead of Pressed / Released
    unsigned long lastHoldTimeMillis;
    // configurable!
    const unsigned long holdDelay = HOLD_DELAY;
    // IMPORTANT! this flag is used to cancel the current operation, if you hold a button down and don't release
    // we don't do anything, so you can read the screen (pressing a button shows the division state for that
    // button -> output mechanism)
    uint8_t heldDownWasPressed = false;

    // TODO? pertains to ClockModeChangeCallback
    uint8_t startupFlagFlipped = false;
    DivisionModeChangeCallback divisionModeChangeCallback = nullptr;// TODO
    DivisionChangeCallback divisionChangeCallback = nullptr;
    ClockModeChangeCallback clockModeChangeCallback = nullptr; // TODO

    // mock millis() so we can test state machinem we change the value of this in tests.cpp
#ifdef TEST_BUILD
    unsigned long mockMillis = 0;
#endif // TEST_BUILD

    unsigned long mMillis() {
#ifdef TEST_BUILD
        return mockMillis;
#else
        return millis();
#endif // TEST_BUILD
    }

//    just print to screen div for current button
    void printCurrentDivisionData() const {
        divisionChangeCallback(ioIndex, false);
    }

    // start counting how long we've been in current state
    void startDebounceTimer() {
        lastStateChangeTimeMillis = mMillis();
    }

    // start counting how long its been since we first held down button
    void startHoldTimer() {
        lastHoldTimeMillis = mMillis();
    }

    // check if we've been in whatever state we are in long enough to consider ourselves actuallin in that state
    bool debounceTimerTimedOut() {
        unsigned long totalMillisInNewState = mMillis() - lastStateChangeTimeMillis;
        return totalMillisInNewState > DEBOUNCE_DELAY;
    }

    // check if user held down button for long enough
    bool holdTimerTimedOut() {
        unsigned long totalMillisSinceFirstHeldDown = mMillis() - lastHoldTimeMillis;
        return totalMillisSinceFirstHeldDown > holdDelay;
    }

    // flip on the flag that disables division increment
    void considerHeldDown() {
        heldDownWasPressed = true;
    }

    // flip off the flag that disables division increment
    void considerNotHeldDown() {
        heldDownWasPressed = false;
    }

    void exitStateMachine() {
        state = State::Released;
    }

    void enterPressedState() {
        state = State::Pressed;
        startHoldTimer();
        considerNotHeldDown();
        printCurrentDivisionData();
    }

    ButtonState(GPIOIndex ioIndex) : ioIndex(ioIndex) {}

    void updateState(uint8_t pinValue) {
        switch (state) {
            case State::Released:
                if (pinValue == PRESSED) {
                    state = State::DebouncePress;
                    startDebounceTimer();
                }
                break;
            case State::DebouncePress:
                if (debounceTimerTimedOut()) {
                    if (pinValue == PRESSED) {
                        enterPressedState();
                    } else {
                        exitStateMachine();
                    }
                }
                break;
            case State::Pressed:
                if (pinValue == RELEASED) {
                    state = State::DebounceRelease;
                    startDebounceTimer();
                } else {
                    if (holdTimerTimedOut()) {
                        state = State::HeldDown;
                        considerHeldDown();
                    }
                }
                break;
            case State::HeldDown:
                if (pinValue == RELEASED) {
                    state = State::DebounceRelease;
                    startDebounceTimer();
                    if (!startupFlagFlipped && ioIndex == GPIOIndex::ONE) {
                        // TODO
                        clockModeChangeCallback(ClockMode::External);
                    }
                }
                break;
            case State::DebounceRelease:
                if (debounceTimerTimedOut()) {
                    if (pinValue == RELEASED) {
                        state = State::Released;
                        if (!heldDownWasPressed) {
                            divisionChangeCallback(ioIndex, true);
                        }
                    } else {
                        enterPressedState();
                    }
                }
                break;
        }
    }
};

#endif //CLK_BUTTON_STATE_H
