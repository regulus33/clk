//
// Created by zack on 9/28/23.
//

#ifndef CLK_CLOCK_MANAGER_H
#define CLK_CLOCK_MANAGER_H

struct ClockManager {
public:
    Division division1;
    Division division2;
    Division division3;
    Division division4;

    void tick() {
        // Clear all bits first
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));

        // Set bits according to the tick() methods
        if (division1.tick()) PORTB |= (1 << PORTB0);
        if (division2.tick()) PORTB |= (1 << PORTB1);
        if (division3.tick()) PORTB |= (1 << PORTB2);
        if (division4.tick()) PORTB |= (1 << PORTB3);
    }
};

#endif //CLK_CLOCK_MANAGER_H
