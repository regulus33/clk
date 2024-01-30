//
// Created by zachary rowden on 07.12.2023.
//

#ifndef CLK_BUTTON_SERVICE_H
#define CLK_BUTTON_SERVICE_H

#include <Arduino.h>
#include "development/debug_utils.h"
#include "development/mock_pind.h"

//Either use the PIND mock value if in test or the actual arduino hardware PIND.
#ifdef TEST_BUILD
#define MY_PIND MockPinD::getInstance()->getMockValue()
#else
#define MY_PIND PIND
#endif


constexpr uint8_t BUTTON1 = 4;
constexpr uint8_t BUTTON2 = 5;
constexpr uint8_t BUTTON3 = 6;
constexpr uint8_t BUTTON4 = 7;
constexpr uint8_t BUTTON_LOOP_BOUNDARY = (BUTTON4 + 1);

// Basically just reads pins. Unfortunately, ButtonState was supposed to be without any methods and the service classes
// would contain all functions. At the moment, there is a lot of complexity in ButtonState TODO: move that here..
class ButtonService {
public:
    // ready up the pins
    static void setup() {
        for (int pin = BUTTON1; pin < BUTTON_LOOP_BOUNDARY; pin++) { pinMode(pin, INPUT_PULLUP); }
        DEBUG_PRINT("[BUTTON][");
        DEBUG_PRINTLN("][SETUP] - pins 4 to 7 are now INPUT_PULLUP");
    }

    // use bitmasking to isolate the bit we need in the PIND register and return it.
    static uint8_t readPin(uint8_t pin) {
        switch (pin) {
            case BUTTON1:
                return (MY_PIND & B00010000) >> BUTTON1;
            case BUTTON2:
                return (MY_PIND & B00100000) >>  BUTTON2;
            case BUTTON3:
                return (MY_PIND & B01000000) >> BUTTON3;
            case BUTTON4:
                return (MY_PIND & B10000000) >> BUTTON4;
            default:
                DEBUG_PRINT("[BUTTON_SERVICE][READ_PIN] - unknown pin value");
        }

        DEBUG_PRINT("[BUTTON][ERROR] - readPin() should always return from switch.");
        return 0;
    }
};

#endif //CLK_BUTTON_SERVICE_H
