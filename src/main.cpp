#include "Arduino.h"
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"
#include "buttons.h"
#include "timer_manager.h"
#include "oled_display.h"

/* Global instances */

ClockManager clockManager;
int pulseReceived = 0;
void clockPulseInterrupt() { pulseReceived = 1; };
TimerManager timerManager(30, 24, clockPulseInterrupt);
OledDisplay display;
Buttons buttons(display, clockManager);
unsigned int lastBpm = 120;

void setup() {
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(A0, INPUT);
    display.setup();
    timerManager.begin();
}

void loop() {
    if(pulseReceived == 1) {
        clockManager.tick();
        pulseReceived = 0;
    }
    buttons.read();
    timerManager.updateBPMFromA0();
    if(timerManager.bpm != lastBpm) {
        Serial.println("value changed");
        display.printLine(timerManager.bpm);
        lastBpm = timerManager.bpm;
    }
}