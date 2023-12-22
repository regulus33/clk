#define TEST_BUILD

#include "unity.h"
#include "Arduino.h"
#include "state/button_state.h"
#include "hardware/knob_service.h"
#include "division_service.h"
#include "state/program_state.h"
#include "development/mock_pind.h"
#include "hardware/button_service.h"

/* Helpers */

void runDividerTestsDynamically(uint8_t num) {
    DividerState dividerState = {IOIndex::ONE, 0b0000000000000001, num, 0};
    TEST_ASSERT_TRUE(DivisionService::tick(dividerState));
    for (int i = 0; i < num; i++) {
        TEST_ASSERT_FALSE(DivisionService::tick(dividerState));
    }
    TEST_ASSERT_TRUE(DivisionService::tick(dividerState));
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

void releaseButtonPress(ButtonState &buttonState, unsigned long lastTime) {
    buttonState.mockMillis = lastTime + 1;
    buttonState.updateState(RELEASED);
    TEST_ASSERT_EQUAL(ButtonState::State::DebounceRelease, buttonState.state);
}

void commitReleaseButtonPress(ButtonState &buttonState, unsigned long lastTime) {
    buttonState.mockMillis = lastTime + DEBOUNCE_DELAY + 1;
    buttonState.updateState(RELEASED);
    TEST_ASSERT_EQUAL(ButtonState::State::Released, buttonState.state);
}

/* Tests */

// ButtonState
void test_transition_to_debounce_press() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
    pressButton(buttonState);
}

// ButtonState
void test_transition_to_pressed() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
    pressButton(buttonState);
    commitButtonPress(buttonState);
}

// ButtonState
void test_transition_to_held() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
}

// ButtonState
void test_transition_to_debounce_release() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
    releaseButtonPress(buttonState, buttonState.mockMillis);
}

// ButtonState
void test_transition_to_release() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
    pressButton(buttonState);
    commitButtonPress(buttonState);
    holdButtonPress(buttonState);
    releaseButtonPress(buttonState, buttonState.mockMillis);
    commitReleaseButtonPress(buttonState, buttonState.mockMillis);
}


// ButtonState
void test_transition_to_press_with_jitter() {
    ButtonState buttonState = ButtonState(IOIndex::ONE);
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
void test_pulse_received_handling() {
    ProgramState programState;
    programState.setPulseReceived(1);
    TEST_ASSERT_EQUAL(programState.getPulseReceived(), 1);
}

void test_io_index_assignment() {
    ProgramState programState;
    TEST_ASSERT_EQUAL(programState.getDivider(0).ioIndex, IOIndex::ONE);
    TEST_ASSERT_EQUAL(programState.getDivider(1).ioIndex, IOIndex::TWO);
    TEST_ASSERT_EQUAL(programState.getDivider(2).ioIndex, IOIndex::THREE);
    TEST_ASSERT_EQUAL(programState.getDivider(3).ioIndex, IOIndex::FOUR);
    TEST_ASSERT_EQUAL(programState.getButton(0).ioIndex, IOIndex::ONE);
    TEST_ASSERT_EQUAL(programState.getButton(1).ioIndex, IOIndex::TWO);
    TEST_ASSERT_EQUAL(programState.getButton(2).ioIndex, IOIndex::THREE);
    TEST_ASSERT_EQUAL(programState.getButton(3).ioIndex, IOIndex::FOUR);
}

// TODO
void test_child_buttons() {
    ProgramState programState;
    TEST_ASSERT_EQUAL(programState.getButton(0).state, ButtonState::State::Released);
}

uint8_t mockPinD() {
    return MockPinD::getInstance()->getMockValue();
}

uint8_t readPinRegister(uint8_t registerMockReturn, uint8_t pin) {
    // Set the mock PIND value to simulate button at pin 4 being pressed
    MockPinD::getInstance()->setMockValue(registerMockReturn);

    return ButtonService::readPin(pin);

}

void runButtonTest(uint8_t expected, uint8_t mock, uint8_t pin) {
    TEST_ASSERT_EQUAL(expected, readPinRegister(mock, pin));
}

void test_button_read_pins_4_to_7() {
    constexpr uint8_t EXAMPLES = 8;

    for (int i = 4; i < EXAMPLES; i++) {
        runButtonTest(0, B00000000, i);
    }

    // Test args
    // 1: expected reading value 2: mock PIND register value 3: pin number
    uint8_t t[EXAMPLES][3]{
            {LOW,  B11101111, 4},
            {LOW,  B11011111, 5},
            {LOW,  B10111111, 6},
            {LOW,  B01111111, 7},
            {HIGH, B00010000, 4},
            {HIGH, B00100000, 5},
            {HIGH, B01000000, 6},
            {HIGH, B10000000, 7},
    };

    for (int i = 0; i < EXAMPLES; i++) {
        runButtonTest(t[i][0], t[i][1], t[i][2]);
    }

}

int runUnityTests(void) {
    UNITY_BEGIN();
    // ButtonState
    RUN_TEST(test_transition_to_debounce_press);
    RUN_TEST(test_transition_to_pressed);
    RUN_TEST(test_transition_to_held);
    RUN_TEST(test_transition_to_debounce_release);
    RUN_TEST(test_transition_to_release);

    // DivisionService
    RUN_TEST(test_different_divider_configurations);

    //KnobService
    RUN_TEST(test_knob_setup);
    RUN_TEST(test_knob_getValue_within_deadzone);
    RUN_TEST(test_knob_getValue_outside_deadzone);

    //ProgramState
    RUN_TEST(test_program_state_constructor);
    RUN_TEST(test_bpm_handling);
    RUN_TEST(test_pulse_received_handling);
    RUN_TEST(test_io_index_assignment);

    // ButtonService
    RUN_TEST(test_button_read_pins_4_to_7);
    UNITY_END();
}

void setup() {
    delay(1000);
    runUnityTests();
}

void loop() {}