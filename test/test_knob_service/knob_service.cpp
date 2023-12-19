#define TEST_BUILD
#include "unity.h"
#include "hardware/knob_service.h"

void test_knob_setup() {
    KnobService::setup();
    // Test if setup initializes the last_adc_value correctly
    TEST_ASSERT_EQUAL_UINT16(LAST_ADC_VALUE, KnobService::getValue(123)); // Pass any mock value
}

void test_knob_getValue_within_deadzone() {
    KnobService::setup();
    // First read with a mock value of 300
    KnobService::getValue(300);
    // Second read with a value within the deadzone (e.g., 310)
    uint16_t val = KnobService::getValue(310);
    // Should still return the first value (300) as 310 is within the deadzone
    TEST_ASSERT_EQUAL_UINT16(300, val);
}

void test_knob_getValue_outside_deadzone() {
    KnobService::setup();
    // First read with a mock value of 300
    KnobService::getValue(300);
    // Second read with a value outside the deadzone (e.g., 400)
    uint16_t val = KnobService::getValue(400);
    // Should return the new value (400) as it's outside the deadzone
    TEST_ASSERT_EQUAL_UINT16(400, val);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_knob_setup);
    RUN_TEST(test_knob_getValue_within_deadzone);
    RUN_TEST(test_knob_getValue_outside_deadzone);
    UNITY_END();
}

void loop() {
    // Not used in this test setup
}