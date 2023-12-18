//
// Created by zack on 9/28/23.
//
#ifndef CLK_CLOCK_MANAGER_H
#define CLK_CLOCK_MANAGER_H
#include "debug_utils.h"

struct ClockManager {
public:
    ClockManager() {}

    Division division1 = Division(8);
    Division division2 = Division(9);
    Division division3 = Division(10);
    Division division4 = Division(11);

    void setup() {
        division1.setup();
        division2.setup();
        division3.setup();
        division4.setup();
        DEBUG_PRINTLN("[CLOCK_MANAGER][SETUP]");
    }

    void tick() {
//        // Clear all bits first
//        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));
//
//        // set various bits corresponding to the tick() state
//        if (division1.tick()) PORTB |= (1 << PORTB0);
//        if (division2.tick()) PORTB |= (1 << PORTB1);
//        if (division3.tick()) PORTB |= (1 << PORTB2);
//        if (division4.tick()) PORTB |= (1 << PORTB3);
    }
};

#endif //CLK_CLOCK_MANAGER_H
