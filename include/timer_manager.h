#ifndef CLK_TIMER_MANAGER_H
#define CLK_TIMER_MANAGER_H
#include <TimerOne.h>
#include <Arduino.h>
#include "state/program_state.h"
#include "development/ram_service.h"
#include "development/debug_utils.h"
#include "hardware/knob_service.h"

class TimerManager {
public:
    static void setup(uint16_t interval_microseconds, void (*onPulseChange)()) {
        Timer1.initialize(interval_microseconds);
        Timer1.attachInterrupt(onPulseChange);
        DEBUG_PRINTLN("[TIMER_MANAGER][SETUP]");
    }

    static uint16_t convertAdcReadToBpm(uint16_t knob_read) {
        return map(knob_read, 0, 1023, 180, 50);
    }

    // Calculate interval in microseconds based on state.bpm and state.ppqn
    static int32_t getTimerIntervalMicroseconds(uint8_t bpm, uint8_t ppqn) {
        return 60L * 1000 * 1000 / bpm / ppqn;
    }

    static void updateTimer1Interval(int32_t timer_interval_microseconds) {
        Timer1.setPeriod(timer_interval_microseconds);
    }


};
#endif // CLK_TIMER_MANAGER_H
