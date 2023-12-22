//
// Created by zachary rowden on 07.12.2023.
//

#ifndef CLK_BUTTON_SERVICE_H
#define CLK_BUTTON_SERVICE_H
#define BUTTON_COUNT 4

#include <Arduino.h>
#include "development/debug_utils.h"
#include "development/mock_pind.h"

#ifdef TEST_BUILD
#define MY_PIND MockPinD::getInstance()->getMockValue()
#else
#define MY_PIND PIND
#endif

class ButtonService {
public:
    static void setup() {
        for (int pin = 4; pin < 8; pin++) {
            pinMode(pin, INPUT_PULLUP);
        }

        DEBUG_PRINT("[BUTTON][");
        DEBUG_PRINTLN("][SETUP] - pins 4 to 7 are now INPUT_PULLUP");
    }

    static uint8_t readPin(uint8_t pin) {
        switch (pin) {
            case 4:
                return (MY_PIND & B00010000) >> 4;
            case 5:
                return (MY_PIND & B00100000) >> 5;
            case 6:
                return (MY_PIND & B01000000) >> 6;
            case 7:
                return (MY_PIND & B10000000) >> 7;
            default:
                DEBUG_PRINT("[BUTTON_SERVICE][READ_PIN] - unknown pin value");
        }
    }
};

#endif //CLK_BUTTON_SERVICE_H
