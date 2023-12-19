#define TEST_BUILD
#include "unity.h"
#include "Arduino.h"
#include "program_state.h"

void expectEqual(int a, int b) {
    TEST_ASSERT_EQUAL(b, a);
}

void test_program_state_constructor() {
    ProgramState programState;
    expectEqual(programState.getDivider(0).indexEndOfSteps, 2);
    expectEqual(programState.getDivider(0).indexSteps, 0);
    expectEqual(programState.getDivider(0).steps, 0b0000000000000001);
}

void test_bpm_handling() {
    ProgramState programState;
    programState.setBpm(120);
    expectEqual(programState.getBpm(), 120);
    expectEqual(programState.bpmChanged(), true);
}

void test_ppqn_handling() {
    ProgramState programState;
    programState.setPpqn(48);
    expectEqual(programState.getPpqn(), 48);
    expectEqual(programState.ppqnChanged(), true);
}

void test_pulse_received_handling() {
    ProgramState programState;
    programState.setPulseReceived(1);
    expectEqual(programState.getPulseReceived(), 1);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_program_state_constructor);
    RUN_TEST(test_bpm_handling);
    RUN_TEST(test_ppqn_handling);
    RUN_TEST(test_pulse_received_handling);
    UNITY_END();
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    runUnityTests();
}

void loop() {

}
