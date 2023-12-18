#define UNITY_OUTPUT_CHAR(a) DEBUG_PRINT(a)
#include "unity.h"
#include "Arduino.h"
#include "button.h"


void expectEqual(int a, int b) {
    TEST_ASSERT_EQUAL(b, a);
}

void test_1() {
    expectEqual(0, 0);
}


int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_1);
}


void setup() {
    Serial.begin(9600);
    delay(1000);
    runUnityTests();

}

void loop() {

}
