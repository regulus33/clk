#define DEBUG
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
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("[MAIN][SETUP] - setting pinMode OUTPUT on 8,9,10,11"));
    RamService::printMemoryUsage();
#endif
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(A0, INPUT);
    display.setup();
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("[MAIN][SETUP] - init display"));
    RamService::printMemoryUsage();
#endif
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
#ifdef DEBUG
        Serial.print(F("[MAIN][LOOP] - pot value changed, printing: "));
        Serial.print(timerManager.bpm);
        Serial.println(F(""));
#endif

        display.printLine(timerManager.bpm, BPM);
        lastBpm = timerManager.bpm;
    }
}