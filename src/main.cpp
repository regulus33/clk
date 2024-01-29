#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"

#define INITIAL_INTERVAL 1000

// Initialize container for all our program state
ProgramState state;

// create the main callback aka interrupt service routine for the clock generator.
void pulseChangeCallback() {
    state.setPulseReceived(1);
}

// TODO
void divisionModeChangeCallback(DivisionMode divisionMode, IOIndex ioIndex) {
    // TODO use ioIndex to locate the divider in programState
    /// TODO update the DivisionMode of each divider (divisionMode member needs to be made still)
}

// create the clock divider division change callback
void divisionChangeCallback(IOIndex ioIndex, uint8_t incrementEndOfSteps) {
    uint8_t  divisionPrint;
    // get the divider based on the ioIndex which is passed in via an instance variable on the DivisionState instance.
    DivisionState &divisionState = state.getDivider(ioIndex);
    // Since we also use this method to only print to the display and not increment, we check if an argument
    // to disable incrementation was provided.
    if (incrementEndOfSteps == true) {
        // get the "string" like thing to print to the oled display representing the divider and it's new state
        divisionPrint = DivisionService::incrementIndexEndOfSteps(divisionState);
    } else {
        // only print the current state of the divider
        divisionPrint = DivisionService::getDivisionPrint(divisionState);
    }

    // print the above mentioned string
    DisplayService::printLine(divisionPrint, PrintType::Div, ioIndex);
}

// TODO NOT IMPLEMENTED
void clockModeChangeCallback(ClockMode clockMode) {
    state.setClockMode(clockMode);
}

void setup() {
    // sets up some logging stuff if building with debug flags
    DEBUG_SETUP;
    // sets up pins and I2C stuff for the display
    DisplayService::setup();
    //
    KnobService::setup();
    ButtonService::setup();
    TimerManager::setup(INITIAL_INTERVAL, pulseChangeCallback);
    DivisionService::setup();
    state.setBpm(120);
    state.setButtonTriggeredCallbacks(
            divisionModeChangeCallback,
            divisionChangeCallback,
            clockModeChangeCallback
    );
    DEBUG_MEMPRINT;
}

void loop() {
    if (state.getPulseReceived() == 1) {
        /* CLEAR BITS IN PORTB REGISTER */
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));

        if (DivisionService::tick(state.getDivider(IOIndex::ONE))) {
            PORTB |= (1 << PORTB0);
        }

        if (DivisionService::tick(state.getDivider(IOIndex::TWO))) {
            PORTB |= (1 << PORTB1);
        }

        if (DivisionService::tick(state.getDivider(IOIndex::THREE))) {
            PORTB |= (1 << PORTB2);
        }

        if (DivisionService::tick(state.getDivider(IOIndex::FOUR))) {
            PORTB |= (1 << PORTB3);
        }

        state.setPulseReceived(0);
    }

    state.setBpm(TimerManager::convertAdcReadToBpm(
            KnobService::getValue())
    );
    if (state.bpmChanged()) {
        uint16_t timer_interval = TimerManager::getTimerIntervalMicroseconds(
                state.getBpm(),
                state.getPpqn()
        );
        TimerManager::updateTimer1Interval(timer_interval);
        DisplayService::printLine(state.getBpm(), BPM);
    }

    state.getButton(IOIndex::ONE).updateState(
            ButtonService::readPin(7)
    );
    state.getButton(IOIndex::TWO).updateState(
            ButtonService::readPin(6)
    );
    state.getButton(IOIndex::THREE).updateState(
            ButtonService::readPin(5)
    );
    state.getButton(IOIndex::FOUR).updateState(
            ButtonService::readPin(4)
    );
}

