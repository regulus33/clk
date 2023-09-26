//
// Created by zack on 9/26/23.
//
//
// Created by zack on 9/16/23.
//
//
// Created by zack on 9/2/23.
//

#include "unity.h"
#include "Arduino.h"

void test_tick() {
    TEST_ASSERT_EQUAL(0, 0);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tick);
    return UNITY_END();
}

/**
  * For Arduino framework
  */
void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);
    runUnityTests();
}

void loop() {

}
