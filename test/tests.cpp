#define TEST_BUILD

#include "unity.h"
#include "Arduino.h"
#include "state/button_state.h"
#include "hardware/knob_service.h"
#include "division.h"
#include "state/program_state.h"

/* Helpers */

void runDividerTestsDynamically(uint8_t num) {
    DividerState dividerState = {0b0000000000000001, num, 0};
    TEST_ASSERT_TRUE(Division::tick(dividerState));
    for (int i = 0; i < num; i++) {
        TEST_ASSERT_FALSE(Division::tick(dividerState));
    }
    TEST_ASSERT_TRUE(Division::tick(dividerState));
}

void updateButton(
        ButtonState &buttonState,
        unsigned long timePassedMillis,
        uint8_t pinPosition,
        ButtonState::State expectedState
) {
    buttonState.mockMillis = timePassedMillis;
    buttonState.updateState(pinPosition);
    TEST_ASSERT_EQUAL(expectedState, buttonState.state);
}

void pressButton(ButtonState &buttonState) {
    updateButton(
            buttonState,
            0,
            PRESSED,
            ButtonState::State::DebouncePress
    );
}

void commitButtonPress(ButtonState &buttonState) {
    updateButton(
            buttonState,
            DEBOUNCE_DELAY + 1,
            PRESSED,
            ButtonState::State::Pressed
    );
}

void holdButtonPress(ButtonState &buttonState) {
    buttonState.mockMillis = buttonState.mockMillis + HOLD_DELAY + 1; // hold for 1000 milliseconds
    buttonState.updateState(PRESSED);
    TEST_ASSERT_EQUAL(ButtonState::State::HeldDown, buttonState.state);
}

void releaseButtonPress(ButtonState& buttonState, unsigned long lastTime) {
    buttonState.mockMillis = lastTime + 1;
    buttonState.updateState(RELEASED);
    TEST_ASSERT_EQUAL(ButtonState::State::DebounceRelease, buttonState.state);
}

void commitReleaseButtonPress(ButtonState& buttonState, unsigned long lastTime) {
    buttonState.mockMillis = lastTime + DEBOUNCE_DELAY + 1;
    buttonState.updateState(RELEASED);
    TEST_ASSERT_EQUAL(ButtonState::State::Released, buttonState.state);
}

/* Tests */

// ButtonState
void test_transition_to_debounce_press() {
    ButtonState buttonState;
    pressButton(buttonState);
}

// ButtonState
void test_transition_to_pressed() {
    ButtonState buttonState;
    pressButton(buttonState);
    commitButtonPress(buttonState);
}

// ButtonState
void test_transition_to_held() {
    ButtonState buttonState;
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
}

// ButtonState
void test_transition_to_debounce_release() {
    ButtonState buttonState;
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
    releaseButtonPress(buttonState, buttonState.mockMillis);
}

// ButtonState
void test_transition_to_release() {
    ButtonState buttonState;
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
    releaseButtonPress(buttonState, buttonState.mockMillis);
    commitReleaseButtonPress(buttonState, buttonState.mockMillis);
}


// ButtonState
void test_transition_to_press_with_jitter() {
    ButtonState buttonState;
    pressButton(buttonState);
}

// Divider
void test_different_divider_configurations() {
    for (int i = 0; i < 16; i++) {
        runDividerTestsDynamically(i);
    }
}

// KnobService
void test_knob_setup() {
    KnobService::setup();
    // Test if setup initializes the last_adc_value correctly
    TEST_ASSERT_EQUAL_UINT16(LAST_ADC_VALUE, KnobService::getValue(123)); // Pass any mock value
}

// KnobService
void test_knob_getValue_within_deadzone() {
    KnobService::setup();
    // First read with a mock value of 300
    KnobService::getValue(300);
    // Second read with a value within the deadzone (e.g., 310)
    uint16_t val = KnobService::getValue(310);
    // Should still return the first value (300) as 310 is within the deadzone
    TEST_ASSERT_EQUAL_UINT16(300, val);
}

// KnobService
void test_knob_getValue_outside_deadzone() {
    KnobService::setup();
    // First read with a mock value of 300
    KnobService::getValue(300);
    // Second read with a value outside the deadzone (e.g., 400)
    uint16_t val = KnobService::getValue(400);
    // Should return the new value (400) as it's outside the deadzone
    TEST_ASSERT_EQUAL_UINT16(400, val);
}

// ProgramState
void test_program_state_constructor() {
    ProgramState programState;
    TEST_ASSERT_EQUAL(programState.getDivider(0).indexEndOfSteps, 2);
    TEST_ASSERT_EQUAL(programState.getDivider(0).indexSteps, 0);
    TEST_ASSERT_EQUAL(programState.getDivider(0).steps, 0b0000000000000001);
}

// ProgramState
void test_bpm_handling() {
    ProgramState programState;
    programState.setBpm(120);
    TEST_ASSERT_EQUAL(programState.getBpm(), 120);
    TEST_ASSERT_EQUAL(programState.bpmChanged(), true);
}

// ProgramState
void test_ppqn_handling() {
    ProgramState programState;
    programState.setPpqn(48);
    TEST_ASSERT_EQUAL(programState.getPpqn(), 48);
    TEST_ASSERT_EQUAL(programState.ppqnChanged(), true);
}

// ProgramState
void test_pulse_received_handling() {
    ProgramState programState;
    programState.setPulseReceived(1);
    TEST_ASSERT_EQUAL(programState.getPulseReceived(), 1);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_transition_to_debounce_press);
    RUN_TEST(test_transition_to_pressed);
    RUN_TEST(test_transition_to_held);
    RUN_TEST(test_transition_to_debounce_release);
    RUN_TEST(test_transition_to_release);
    RUN_TEST(test_different_divider_configurations);
    RUN_TEST(test_knob_setup);
    RUN_TEST(test_knob_getValue_within_deadzone);
    RUN_TEST(test_knob_getValue_outside_deadzone);
    RUN_TEST(test_program_state_constructor);
    RUN_TEST(test_bpm_handling);
    RUN_TEST(test_ppqn_handling);
    RUN_TEST(test_pulse_received_handling);
    UNITY_END();
}

void setup() {
    delay(1000);
    runUnityTests();
}

void loop() {}
