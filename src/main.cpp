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

// TODO CHANGE THE CLOCK SOURCE IN HERE
void divisionModeChangeCallback(DivisionMode divisionMode, GPIOIndex ioIndex) {}

// create the clock divider division change callback
void divisionChangeCallback(GPIOIndex ioIndex, uint8_t incrementEndOfSteps) {
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
    // sets up or own adc logic for faster reads (no HAL used)
    KnobService::setup();
    // Setup pin inputs
    ButtonService::setup();
    // Attach our callback / ISR to the Timer1 library's timer1 service. It will call this function on each clock cycle
    TimerManager::setup(INITIAL_INTERVAL, pulseChangeCallback);
    // set up the pins toward which the OUTPUTS of the program will funnel
    DivisionService::setup();
    // set the initial bpm of the internal clocking mechanism
    state.setBpm(120);
    // set up the callbacks that should be connected to our buttons' state machines. Each button has a state machine
    // that will call these functions at certain key state transition points.
    state.setButtonTriggeredCallbacks(divisionModeChangeCallback, divisionChangeCallback, clockModeChangeCallback);
    // Print out how much memory we've used after all this setup process (if in debug mode)
    DEBUG_MEMPRINT;
}

void loop() {
    // if the clock pulse is positive, we check out "dividers" and write to their bits in the register.
    if (state.getPulseReceived() == 1) {
        // clear the bits in the portB register first i.e. 00000000
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));

        // if the DividerState associated with this GPIO pin is 1, write 1 to the associated bit
        if (DivisionService::tick(state.getDivider(GPIOIndex::ONE))) {
            PORTB |= (1 << PORTB0);
        }

        // if the DividerState associated with this GPIO pin is 2, write 1 to the associated bit
        if (DivisionService::tick(state.getDivider(GPIOIndex::TWO))) {
            PORTB |= (1 << PORTB1);
        }

        // if the DividerState associated with this GPIO pin is 3, write 1 to the associated bit
        if (DivisionService::tick(state.getDivider(GPIOIndex::THREE))) {
            PORTB |= (1 << PORTB2);
        }

        // if the DividerState associated with this GPIO pin is 4, write 1 to the associated bit
        if (DivisionService::tick(state.getDivider(GPIOIndex::FOUR))) {
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

    state.getButton(GPIOIndex::ONE).updateState(
            ButtonService::readPin(7)
    );
    state.getButton(GPIOIndex::TWO).updateState(
            ButtonService::readPin(6)
    );
    state.getButton(GPIOIndex::THREE).updateState(
            ButtonService::readPin(5)
    );
    state.getButton(GPIOIndex::FOUR).updateState(
            ButtonService::readPin(4)
    );
}

