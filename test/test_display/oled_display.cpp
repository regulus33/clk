#include "unity.h"
#include "oled_display.h"

void test_buildPrintString_BPM() {
    char actual_string[17];
    OledDisplay::buildPrintString(BPM, 120, actual_string);
    TEST_ASSERT_EQUAL_STRING("BPM: 120", actual_string);
}

void test_buildPrintString_Div() {
    char actual_string[17];
    OledDisplay::buildPrintString(Div, 8, actual_string);
    TEST_ASSERT_EQUAL_STRING("DIV: 008", actual_string);
}

void test_buildPrintString_Rhythm() {
    char actual_string[17];
    OledDisplay::buildPrintString(Rhythm, 15, actual_string);
    TEST_ASSERT_EQUAL_STRING("BIT: 015", actual_string);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_buildPrintString_BPM);
    RUN_TEST(test_buildPrintString_Div);
    RUN_TEST(test_buildPrintString_Rhythm);
    UNITY_END();
}

void loop() {
    // Not used in this test setup
}
