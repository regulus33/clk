//
// Created by zack on 9/3/23.
//
#ifndef CLK_TIMER_MANAGER_H
#define CLK_TIMER_MANAGER_H

#include <TimerOne.h>
#include <Arduino.h>
#include "program_state.h"
#include "clock_manager.h"
#include "ram_service.h"
#include "debug_utils.h"
#include "knob.h"

class TimerManager {
public:
   static void setup(uint16_t interval_microseconds,  void (*pulse_callback)()) {
        Timer1.initialize(interval_microseconds);
        Timer1.attachInterrupt(pulse_callback);
        DEBUG_PRINTLN("[TIMER_MANAGER][SETUP]");
    }

    static uint16_t convert_adc_read_to_bpm(uint16_t knob_read) {
        return map(knob_read, 0, 1023, 180, 50);
   }
    // Calculate interval in microseconds based on state.bpm and state.ppqn
    static int32_t get_timer_interval_microseconds(uint8_t bpm, uint8_t ppqn) {
        return 60L * 1000 * 1000 / bpm / ppqn;
    }

    static void update_timer1_interval(int32_t timer_interval_microseconds) {
        Timer1.setPeriod(timer_interval_microseconds);
    }


};

#endif // CLK_TIMER_MANAGER_H
