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
        PORTB = 0x00;
        division1.tick() ? PORTB ^= (1 << PORTB0) : 0;
        division2.tick() ? PORTB ^= (1 << PORTB1) : 0;
        division3.tick() ? PORTB ^= (1 << PORTB2) : 0;
        division4.tick() ? PORTB ^= (1 << PORTB3) : 0;
    }
};

#endif //CLK_CLOCK_MANAGER_H
