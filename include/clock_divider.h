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


class ClockDivider {
private:
    ProgramState &state;
    DivisionChangeCallback divisionChangeCallback = nullptr;
    PulseChangeCallback pulseChangeCallback = nullptr;
    DivisionModeChangeCallback divisionModeChangeCallback = nullptr;
    ClockModeChangeCallback clockModeChangeCallback = nullptr;
public:
#ifdef TEST_BUILD
    // FOR testing, simply do clockDivider.writeToPortStub = myFunctionPointer
    // it calls the function with arg GPIOIndex of the division state it is positivly pulsing
    WriteToPortStub writeToPortStub = nullptr;
#endif

    ProgramState &getState() const { return state; }

    ClockDivider(
            ProgramState &initialState,
            void(*initialDivisionChangeCallback)(GPIOIndex, uint8_t),
            void(*initialPulseChangeCallback)(),
            void(*initialDivisionModeChangeCallback)(DivisionMode, GPIOIndex),
            void(*initialClockModeChangeCallback)(ClockMode)
    ) : state(initialState),
        divisionChangeCallback(initialDivisionChangeCallback),
        pulseChangeCallback(initialPulseChangeCallback),
        divisionModeChangeCallback(initialDivisionModeChangeCallback),
        clockModeChangeCallback(initialClockModeChangeCallback) {}

    void setup() {
        state.setButtonTriggeredCallbacks(
                divisionModeChangeCallback, divisionChangeCallback, clockModeChangeCallback);
    }


    void pulseGpioPin(uint8_t registerLocation, GPIOIndex gpioIndex) {
        // get the divider state based on the IoIndex
        // pass the divider state to the DivisionService::tick method
        // if the return value of tick is TRUE
        // make the PORTB bit associated with the pin this DivisionState is focused on 1
        if (DivisionService::tick(state.divisionStateAtIndex(gpioIndex))) {
#ifdef TEST_BUILD
            writeToPortStub(gpioIndex);
#else
            PORTB |= (1 << registerLocation);
#endif // TEST_BUILD
        }
    }


// clear the bits in the portB register 00000000
    void clearBits() {
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));
    }

    void loop() {
        // if the clock pulse is positive, we check out "dividers" and write to their bits in the register.
        if (state.getPulseReceived()) {
            clearBits();
            pulseGpioPin(PORTB0, GPIOIndex::ONE);
            pulseGpioPin(PORTB1, GPIOIndex::TWO);
            pulseGpioPin(PORTB2, GPIOIndex::THREE);
            pulseGpioPin(PORTB3, GPIOIndex::FOUR);
            // Set pulse state back to 0
            // NOTE this means even if we were to pulse the clock on every trigger or DivisionService.tick
            // we would still be clocking less than 1/2 of the pulse because we only run this code when pulse
            // is 1 which is every other clock signal.
            state.setPulseReceived(0);
        }

        // take the adc value (usually something between 0 and ~1000 and map it ot bpm
        // then set that bpm as the bpm for program state
        state.setBpm(TimerManager::convertAdcReadToBpm(
                KnobService::getValue())
        );

        // if bpm has changed update the timer to match the bpm
        if (state.bpmChanged()) {
            // get the interval in microseconds that should be between each pulse
            uint16_t timer_interval = TimerManager::getTimerIntervalMicroseconds(
                    state.getBpm(),
                    PPQN
            );
            // insert that interval
            TimerManager::updateTimer1Interval(timer_interval);
            // print the change to the output
            DisplayService::updateOled(state.getBpm(), BPM);
        }
        // get button read pin value and pass it to the state machine in ButtonState
        state.buttonStateAtIndex(GPIOIndex::ONE).updateState(ButtonService::readPin(7));
        state.buttonStateAtIndex(GPIOIndex::TWO).updateState(ButtonService::readPin(6));
        state.buttonStateAtIndex(GPIOIndex::THREE).updateState(ButtonService::readPin(5));
        state.buttonStateAtIndex(GPIOIndex::FOUR).updateState(ButtonService::readPin(4));
    }
};


#endif //CLK_CLOCK_DIVIDER_H