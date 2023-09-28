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
#include "output.h"
#include "division.h"
#include "clock_manager.h"

void expectEqual(int a, int b) {
    TEST_ASSERT_EQUAL(b, a);
}

void resetOutputPins() {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

// ACTUAL TESTS ========
void testRun_Output_writeOutputs() {
    resetOutputPins();
    Output output;
    output.commitPinStatesToRegister();
    expectEqual(digitalRead(8), LOW);
    expectEqual(digitalRead(9), LOW);
    expectEqual(digitalRead(10), LOW);
    expectEqual(digitalRead(11), LOW);
}

void testRun_Output_toggleOutput() {
    /* reset pin states */
    resetOutputPins();
    Output output;
    output.commitPinStatesToRegister();
    expectEqual(digitalRead(8), LOW);
    expectEqual(digitalRead(9), LOW);
    expectEqual(digitalRead(10), LOW);
    expectEqual(digitalRead(11), LOW);
    output.toggleBit0();
    output.toggleBit1();
    output.toggleBit2();
    output.toggleBit3();
    output.commitPinStatesToRegister();
    expectEqual(digitalRead(8), HIGH);
    expectEqual(digitalRead(9), HIGH);
    expectEqual(digitalRead(10), HIGH);
    expectEqual(digitalRead(11), HIGH);
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

void test_ClockManager_update() {
    resetOutputPins();
    ClockManager clockManager;
    expectEqual(clockManager.output.pinStates, 0b00000000);
    clockManager.updateState();
    expectEqual(clockManager.output.pinStates, 0b00001111);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    long now = micros();
    RUN_TEST(testRun_Output_writeOutputs);
    RUN_TEST(testRun_Output_toggleOutput);
    RUN_TEST(testRun_Division_tick1);
    RUN_TEST(testRun_Division_tick2);
    RUN_TEST(testRun_Division_tick3);
    RUN_TEST(test_ClockManager_update);
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
