//
// Created by zack on 9/28/23.
//

#ifndef CLK_CLOCK_MANAGER_H
#define CLK_CLOCK_MANAGER_H
struct ClockManager {
public:
    Output output;
    Division division1;
    Division division2;
    Division division3;
    Division division4;

    void increment1() {
        division1.incrementDiv();
    }

    void increment2() {
        division2.incrementDiv();
    }

    void increment3() {
        division3.incrementDiv();
    }

    void increment4() {
        division4.incrementDiv();
    }

    // this needs to run over and over again in loop to keep the state updated
    void updateState() {
        if(division1.willTick()) {
            output.toggleBit0();
        };
        if(division2.willTick()) {
            output.toggleBit1();
        }
        if(division3.willTick()) {
            output.toggleBit2();
        }
        if(division3.willTick()) {
            output.toggleBit3();
        }
    }

    void tick() {

    }
};
#endif //CLK_CLOCK_MANAGER_H
