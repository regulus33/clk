#define TEST_BUILD
#include "unity.h"
#include "Arduino.h"
#include "division.h"


void divider_with_num(uint8_t num) {
    DividerState dividerState = {0b0000000000000001,  num, 0};
    TEST_ASSERT_TRUE(Division::tick(dividerState));
    for(int i = 0; i < num; i++) {
        TEST_ASSERT_FALSE(Division::tick(dividerState));
    }
    TEST_ASSERT_TRUE(Division::tick(dividerState));
}

void test_different_divider_configurations() {
        for(int i = 0; i < 16; i++) {
            divider_with_num(i);
        }
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_different_divider_configurations);
    UNITY_END();
}

void loop() {
    // Not used in this test setup
}
