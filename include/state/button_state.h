//
// Created by zachary rowden on 19.12.2023.
//

#ifndef CLK_BUTTON_STATE_H
#define CLK_BUTTON_STATE_H

#include "Arduino.h"
#include "development/debug_utils.h"

constexpr uint8_t PRESSED = 0;
constexpr uint8_t RELEASED = 1;

struct ButtonState {
    enum class State {
        Released,
        DebouncePress,
        Pressed,
        HeldDown,
        DebounceRelease
    };
    State state = State::Released;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = 50;
    unsigned long lastHoldTime{};
    const unsigned long holdDelay = 1000;
#ifdef TEST_BUILD
    unsigned long mockMillis = 0;
#endif

    unsigned long mMillis() {
#ifdef TEST_BUILD
        return mockMillis;
#else
        return mMillis();
#endif
    }

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
                }

                break;
            case State::DebounceRelease:
                if ((mMillis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == RELEASED) {
                        state = State::Released;
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
