//
// Created by zack on 9/28/23.
//
#include <Arduino.h>

#ifndef CLK_OUTPUT_H
#define CLK_OUTPUT_H
struct Output {
public:
    uint8_t pinStates = 0b00000000;
    void commitPinStatesToRegister() const {
        PORTB = (PORTB & 0xF0) | (pinStates & 0x0F);
    }

    void toggleBit0() {
        pinStates ^= (1 << 0);
    }

    void toggleBit1() {
        pinStates ^= (1 << 1);
    }

    void toggleBit2() {
        pinStates ^= (1 << 2);
    }

    void toggleBit3() {
        pinStates ^= (1 << 3);
    }
};
#endif //CLK_OUTPUT_H
