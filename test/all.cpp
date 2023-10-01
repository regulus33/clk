//
// Created by zack on 9/26/23.
//
//
// Created by zack on 9/16/23.
//
//
// Created by zack on 9/2/23.
//
#define UNITY_OUTPUT_CHAR(a) DEBUG_PRINT(a)
#include "unity.h"
#include "Arduino.h"
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"



void expectEqual(int a, int b) {
    TEST_ASSERT_EQUAL(b, a);
}

void resetOutputPins() {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}


void testRun_Division_tick1() {
    Division division;
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 1);
}

void testRun_Division_tick2() {
    Division division;
    division.incrementDiv();
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 1);
}

void testRun_Division_tick3() {
    Division division;
    division.incrementDiv();
    division.incrementDiv();
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 1);
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 0);
    expectEqual(division.tick(), 1);
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

void test_ClockManager_tick_WHEN_DIVS_3333() {
    resetOutputPins();
    ClockManager clockManager;
    DummyTestClock dummyTestClock(clockManager, 3, 3, 3, 3);
    dummyTestClock.fakeClockCycles8();
    expectEqual(dummyTestClock.pinStateSnapshots[0], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[1], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[2], 0b00001111);
    expectEqual(dummyTestClock.pinStateSnapshots[3], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[4], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[5], 0b00001111);
    expectEqual(dummyTestClock.pinStateSnapshots[6], 0b00000000);
    expectEqual(dummyTestClock.pinStateSnapshots[7], 0b00000000);

}


int runUnityTests(void) {
    UNITY_BEGIN();
    long now = micros();
    RUN_TEST(testRun_Division_tick1);
    RUN_TEST(testRun_Division_tick2);
    RUN_TEST(testRun_Division_tick3);
    RUN_TEST(test_ClockManager_tick_WHEN_DIVS_1111);
    RUN_TEST(test_ClockManager_tick_WHEN_DIVS_2222);
    long later = micros();
    long total_time = later - now;
    Serial.println("TOTAL TIME:========");
    Serial.println(total_time);
    return UNITY_END();
}

/**
  * For Arduino framework
  */
void setup() {
    Serial.begin(9600);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);
    runUnityTests();

}

void loop() {

}
