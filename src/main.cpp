#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"

void setup() {
 // initialize the avr-debugger
    debug_init();
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
}

/*
 *          TESTS
 */

bool expectEqual(int a, int b) {
    if(a != b) {
        return false;
    }
    return a == b;
}

void resetOutputPins() {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

void test_ClockManager_tick_WHEN_DIVS_1111() {
    resetOutputPins();
    ClockManager clockManager;
    DummyTestClock dummyTestClock(clockManager, 1, 1, 1, 1);
    dummyTestClock.fakeClockCycles8();
    for(int i = 0; i < 8; i++) {
        for(int d = 0; d < 4; d++) {
            expectEqual(dummyTestClock.pinStateSnapshots[i], 0b00001111);
        }
    }
}

void test_ClockManager_tick_WHEN_DIVS_2222() {
    resetOutputPins();
    ClockManager clockManager;
    DummyTestClock dummyTestClock(clockManager, 2, 2, 2, 2);
    dummyTestClock.fakeClockCycles8();
    expectEqual(dummyTestClock.pinStateSnapshots[0], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[1], 0b00001111);
    expectEqual(dummyTestClock.pinStateSnapshots[2], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[3], 0b00001111);
    expectEqual(dummyTestClock.pinStateSnapshots[4], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[5], 0b00001111);
    expectEqual(dummyTestClock.pinStateSnapshots[6], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[7], 0b00001111);

}


ClockManager clockManager;
void loop() {

//    testRun_Output_writeOutputs();
//    testRun_Output_toggleOutput();
//    testRun_Division_tick1();
//    testRun_Division_tick2();
//    testRun_Division_tick3();
//    test_ClockManager_update();
//    test_ClockManager_tick_WHEN_DIVS_1111();
    test_ClockManager_tick_WHEN_DIVS_2222();

}