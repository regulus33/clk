//
// Created by zachary rowden on 31.01.2024.
//

#ifndef CLK_CLOCK_DIVIDER_H
#define CLK_CLOCK_DIVIDER_H
//
// Created by zachary rowden on 07.12.2023.
//

#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"
#include "config.h"
#include "callbacks.h"

#define INITIAL_INTERVAL 1000

#ifndef CLOCK_DIVIDER
#define CLOCK_DIVIDER

class ClockDivider {
private:
    ProgramState& state;
    DivisionChangeCallback& divisionChangeCallback;
    PulseChangeCallback& pulseChangeCallback;
    DivisionModeChangeCallback& divisionModeChangeCallback;
    ClockModeChangeCallback& clockModeChangeCallback;

public:
    ProgramState& getState() const { return state; }
    DivisionChangeCallback& getDivisionChangeCallback() const { return divisionChangeCallback; }
    PulseChangeCallback& getPulseChangeCallback() const { return pulseChangeCallback; }
    DivisionModeChangeCallback& getDivisionModeChangeCallback() const { return divisionModeChangeCallback; }
    ClockModeChangeCallback& getClockModeChangeCallback() const { return clockModeChangeCallback; }

    // Constructor
    ClockDivider(
             ProgramState& initialState,
             DivisionChangeCallback& initialDivisionChangeCallback,
             PulseChangeCallback& initialPulseChangeCallback,
             DivisionModeChangeCallback& initialDivisionModeChangeCallback,
             ClockModeChangeCallback& initialClockModeChangeCallback
    ) : state(initialState),
        divisionChangeCallback(initialDivisionChangeCallback),
        pulseChangeCallback(initialPulseChangeCallback),
        divisionModeChangeCallback(initialDivisionModeChangeCallback),
        clockModeChangeCallback(initialClockModeChangeCallback)
    {}
};

#endif //CLOCK_DIVIDER

#endif //CLK_CLOCK_DIVIDER_H