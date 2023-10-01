//
// Created by zack on 10/1/23.
//

#ifndef CLK_DUMMY_TEST_CLOCK_H
#define CLK_DUMMY_TEST_CLOCK_H
struct DummyTestClock {
    ClockManager& clockManager;
    DummyTestClock(ClockManager& clockManager, int div1, int div2, int div3, int div4) : clockManager(clockManager) {
        clockManager.division1.setDivision(div1);
        clockManager.division2.setDivision(div2);
        clockManager.division3.setDivision(div3);
        clockManager.division4.setDivision(div4);
    };
    // Declare a function pointer type
    typedef uint8_t (*TestFunction)();

    /* keep each result of pinStates here */

    uint8_t pinStateSnapshots[8] = {
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000
    };

    int pinToIndexMap[4] = {8,9,10,11 };

    /* global tick 8 times and set result in array of bytes representing pin states in each tick */
    void fakeClockCycles8() {
        for (int i = 0; i < 8; i++) {
            clockManager.tick();
            delay(100);
            pinStateSnapshots[i] = PINB;
            // Delay for a short time
            delay(100);
        }
    }
};

#endif //CLK_DUMMY_TEST_CLOCK_H
