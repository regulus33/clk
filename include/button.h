//
// Created by zachary rowden on 07.12.2023.
//

#ifndef CLK_BUTTON_H
#define CLK_BUTTON_H
#include <Arduino.h>

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

    void update() {
        uint8_t pinValue = digitalRead(pin);
        switch (state) {
            case State::Released:
                if (pinValue == HIGH) {
                    state = State::DebouncePress;
                    lastDebounceTime = millis();
                }
                break;
            case State::DebouncePress:
                if ((millis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == HIGH) {
                        state = State::Pressed;
                        lastHoldTime = millis();
                        // TODO: Handle button pressed event
                    } else {
                        state = State::Released;
                    }
                }
                break;
            case State::Pressed:
                if (pinValue == LOW) {
                    // TODO: is this working if we move immediately into State::DebounceRelease
                    // as soon as pinValue is LOW? I don't think so.
                    // It might be as simple as considering State::DebounceRelease as pressed as well
                    state = State::DebounceRelease;
                    lastDebounceTime = millis();
                } else {

                }
                break;
            case State::DebounceRelease:
                if ((millis() - lastDebounceTime) > debounceDelay) {
                    if (pinValue == LOW) {
                        state = State::Released;
                        // TODO: Handle button released event
                    } else {
                        state = State::Pressed;
                        lastHoldTime = millis();
                    }
                }
                break;
        }
    }

    bool isPressed() const {
        // return (state == State::Pressed) || (state == State::DebounceRelease)
        return state == State::Pressed;
    };

    void setupInterrupt(void (*ISR)(void), int mode) {
        attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
    }


private:
    uint8_t pin;
    volatile State state;
    unsigned long lastDebounceTime;
    unsigned long lastHoldTime;
    const unsigned long debounceDelay = 50; // debounce time in milliseconds
    const unsigned long holdDelay = 1000;

    static void handleInterrupt() {
        // This method will be called on interrupt
        // TODO: Update state or set flags as necessary
    }

};
#endif //CLK_BUTTON_H
