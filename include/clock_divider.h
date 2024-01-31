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
    ProgramState state;
    DivisionChangeCallback divisionChangeCallback;
    PulseChangeCallback pulseChangeCallback;
    DivisionModeChangeCallback divisionModeChangeCallback;
    ClockModeChangeCallback clockModeChangeCallback;

public:
    // Getters
    ProgramState getState() const { return state; }
    DivisionChangeCallback getDivisionChangeCallback() const { return divisionChangeCallback; }
    PulseChangeCallback getPulseChangeCallback() const { return pulseChangeCallback; }
    DivisionModeChangeCallback getDivisionModeChangeCallback() const { return divisionModeChangeCallback; }
    ClockModeChangeCallback getClockModeChangeCallback() const { return clockModeChangeCallback; }

    // Setters
    void setState(const ProgramState& newState) { state = newState; }
    void setDivisionChangeCallback(const DivisionChangeCallback& newCallback) { divisionChangeCallback = newCallback; }
    void setPulseChangeCallback(const PulseChangeCallback& newCallback) { pulseChangeCallback = newCallback; }
    void setDivisionModeChangeCallback(const DivisionModeChangeCallback& newCallback) { divisionModeChangeCallback = newCallback; }
    void setClockModeChangeCallback(const ClockModeChangeCallback& newCallback) { clockModeChangeCallback = newCallback; }

    // Constructor
    ClockDivider(
            const ProgramState& initialState,
            const DivisionChangeCallback& initialDivisionChangeCallback,
            const PulseChangeCallback& initialPulseChangeCallback,
            const DivisionModeChangeCallback& initialDivisionModeChangeCallback,
            const ClockModeChangeCallback& initialClockModeChangeCallback
    ) : state(initialState),
        divisionChangeCallback(initialDivisionChangeCallback),
        pulseChangeCallback(initialPulseChangeCallback),
        divisionModeChangeCallback(initialDivisionModeChangeCallback),
        clockModeChangeCallback(initialClockModeChangeCallback)
    {
        // You could call setup here if necessary
    }

    // Rest of the code...

};

#endif //CLOCK_DIVIDER

#endif //CLK_CLOCK_DIVIDER_H