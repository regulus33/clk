#define TEST_BUILD
#include "unity.h"
#include "Arduino.h"
#include "state/button_state.h"

void test_transition_to_debounce_press() {
    ButtonState buttonState;
    buttonState.mockMillis = 0;
    buttonState.updateState(PRESSED); // Simulate button press
    TEST_ASSERT_EQUAL(ButtonState::State::DebouncePress, buttonState.state);
}

void test_transition_to_pressed() {
    ButtonState buttonState;
    buttonState.mockMillis = 0;
    buttonState.updateState(PRESSED);
    buttonState.mockMillis = 51; // Simulate time passage
    buttonState.updateState(PRESSED);
    TEST_ASSERT_EQUAL(ButtonState::State::Pressed, buttonState.state);
}

// Additional tests for other states...
void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_transition_to_debounce_press);
    RUN_TEST(test_transition_to_pressed);
    // Additional test calls...
    UNITY_END();
}

void loop() {
    // Not used in this test setup
}
