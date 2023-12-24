//
// Created by zachary rowden on 19.12.2023.
//
#ifndef CLK_BUTTON_STATE_H
#define CLK_BUTTON_STATE_H

#include "Arduino.h"
#include "development/debug_utils.h"
#include "io_index.h"
#include "clock_mode.h"
#include "division_mode.h"

constexpr uint8_t PRESSED = 0;
constexpr uint8_t RELEASED = 1;
constexpr unsigned long DEBOUNCE_DELAY = 50;
constexpr unsigned long HOLD_DELAY = 1000;
typedef void (*DivisionModeChangeCallback)(DivisionMode, IOIndex);
typedef void (*DivisionChangeCallback)(uint8_t, IOIndex);
typedef void (*ClockModeChangeCallback)(ClockMode);

struct ButtonState {
    //////////////////////////
    // FOR 🍐弐 pairing with Divison State
    IOIndex ioIndex;
    // 💾 STATE
    enum class State {
        Released, DebouncePress, Pressed, HeldDown, DebounceRelease
    };
    State state = State::Released;

    // STATE MACHINE DEBOUNCE 🎾
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = DEBOUNCE_DELAY;
    unsigned long lastHoldTime{};
    const unsigned long holdDelay = HOLD_DELAY;
    //////////////////////////
    // 🚩 for GLOBAL 🌎 CLOCK MODE change, only check once on startup...
    uint8_t startupFlagFlipped = false;
    /////////////////////////
    // ‼️callbacks ☎️
    DivisionModeChangeCallback divisionModeChangeCallback = nullptr;
    DivisionChangeCallback divisionChangeCallback = nullptr;
    ClockModeChangeCallback clockModeChangeCallback = nullptr;

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


    ButtonState(IOIndex ioIndex) : ioIndex(ioIndex) {}

    void updateState(int pinValue) {
        switch (state) {
            case State::Released:
                if (pinValue == PRESSED) {
                    state = State::DebouncePress;
                    lastDebounceTime = mMillis();
                    DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::DebouncePress]");
                }

                break;
            case State::DebouncePress:
                if ((mMillis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == PRESSED) {
                        state = State::Pressed;
                        lastHoldTime = mMillis();
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
                    } else {
                        state = State::Released;
                    }
                }
                break;
            case State::Pressed:
                if (pinValue == RELEASED) {
                    state = State::DebounceRelease;
                    lastDebounceTime = mMillis();
                    DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::DebounceRelease]");
                } else { // pinValue == RELEASED
                    if (mMillis() - lastHoldTime > holdDelay) {
                        state = State::HeldDown;
                        DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::HeldDown]");
                    }
                }
                break;
            case State::HeldDown:
                if (pinValue == RELEASED) {
                    state = State::DebounceRelease;
                    lastDebounceTime = mMillis();
                    DEBUG_PRINTLN("[BUTTON][STATE_CHANGE][State::DebounceRelease]");
                    // ‼️This is how we switch from internal clock to external interrupt (jack in +5v pulse) 🌍
//                    if (!startupFlagFlipped && ioIndex == IOIndex::ONE) {
//                        clockModeChangeCallback(ClockMode::External);
//                    }
                }

                break;
            case State::DebounceRelease:
                if ((mMillis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == RELEASED) {
                        state = State::Released;
//                        divisionChangeCallback(3, ioIndex);
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Released]");
                    } else {
                        state = State::Pressed;
                        lastHoldTime = mMillis();
                        DEBUG_PRINT("[BUTTON][STATE_CHANGE][State::Pressed]");
                    }
                }

                break;
        }
    }

};

#endif //CLK_BUTTON_STATE_H
