//
// Created by zack on 10/15/23.
//
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
#include "oled_display.h"

void expectEqual(int a, int b) {
    TEST_ASSERT_EQUAL(b, a);
}

void testOledDisplayPrint() {

}


int runUnityTests(void) {
    UNITY_BEGIN();
    long now = micros();
    RUN_TEST(testOledDisplayPrint);
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
