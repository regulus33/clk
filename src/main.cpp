#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"
#include "buttons.h"

void setup() {
 // initialize the avr-debugger
    debug_init();
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
}

/*
 *          TESTS
 */

char* toBinaryString(uint8_t value, char* buffer) {
    buffer[8] = '\0';  // Null-terminate the string
    for (int i = 7; i >= 0; --i) {
        buffer[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
    return buffer;
}

bool expectEqual(int a, int b) {
//    char buffer_a[9];
//    toBinaryString(a, buffer_a);
//    char buffer_b[9];
//    toBinaryString(b, buffer_b);
    if(a != b) {
        return false;
    }
    return a == b;
}

void resetOutputPins() {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

void testRun_Buttons_when_pressed_1() {
    Buttons buttons;
    buttons.read();
//    expectEqual(buttons.buttonStates[0], B0000001); // working!
    expectEqual(buttons.buttonPressedStates[0], B0000001);
}

ClockManager clockManager;
void loop() {
//    expectEqual(digitalRead(7), LOW);
//    testRun_Output_writeOutputs();
//    testRun_Output_toggleOutput();
//    testRun_Division_tick1();
//    testRun_Division_tick2();
//    testRun_Division_tick3();
//    test_ClockManager_update();
//    test_ClockManager_tick_WHEN_DIVS_1111();
//    test_ClockManager_tick_WHEN_DIVS_2222();
//    testRun_Buttons_when_pressed_1();


}