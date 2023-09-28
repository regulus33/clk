#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include "output.h"
#include "division.h"
#include "clock_manager.h"


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

bool expectEqual(int a, int b) {
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



void loop() {

//    testRun_Output_writeOutputs();
//    testRun_Output_toggleOutput();
//    testRun_Division_tick1();
//    testRun_Division_tick2();
//    testRun_Division_tick3();
//    test_ClockManager_update();

}