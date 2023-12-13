#include "Arduino.h"
#include "division.h"
#include "clock_manager.h"
#include "dummy_test_clock.h"
#include "button_manager.h"
#include "timer_manager.h"
#include "oled_display.h"
#include "button.h"

/* Global instances */

ClockManager clockManager;
int pulseReceived = 0;

void clockPulseInterrupt() { pulseReceived = 1; };
TimerManager timerManager(30, 24, clockPulseInterrupt);
OledDisplay display;
unsigned int lastBpm = 120;

uint8_t lastPortDState = PIND & B11110000; // Initialize lastPortState
Button buttons[4] = {
        Button(7),
        Button(6),
        Button(5),
        Button(4),
};

void setup() {
#ifdef DEBUG
    RamService::printMemoryUsage();
#endif
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(7, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    display.setup();
//    ButtonManager::setup();
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("[MAIN][SETUP] - init display"));
    RamService::printMemoryUsage();
#endif
    timerManager.begin();
}

void loop() {
    if (pulseReceived == 1) {
        clockManager.tick();
        pulseReceived = 0;
    }

    buttons[0].update();
    buttons[1].update();
    buttons[2].update();
    buttons[3].update();

    timerManager.updateBPMFromA0();
    if (timerManager.bpm != lastBpm) {
#ifdef DEBUG
        Serial.print(F("[MAIN][LOOP] - pot value changed, printing: "));
        Serial.print(timerManager.bpm);
        Serial.println(F(""));
#endif

        display.printLine(timerManager.bpm, BPM);
        lastBpm = timerManager.bpm;
    }
}