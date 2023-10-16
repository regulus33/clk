#include "Arduino.h"
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"
#include "buttons.h"
#include "timer_manager.h"
#include "oled_display.h"

/* Global instances */
ClockManager clockManager;
void clockPulseInterrupt() { clockManager.tick(); }
TimerManager timerManager(30, 24, clockPulseInterrupt);
OledDisplay display;
unsigned int lastBpm = 120;

void setup() {
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(A0, INPUT);
    display.setup();
    display.printLine("penis time");
    timerManager.begin();
}




void loop() {
    timerManager.updateBPMFromA0();
    if(timerManager.bpm != lastBpm) {
        display.printLine(timerManager.bpm);
    }
}