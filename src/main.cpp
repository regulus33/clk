#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"
#include "config.h"
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
    DivisionState &divisionState = state.divisionStateAtIndex(ioIndex);
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

void pulseGpioPin(uint8_t registerLocation, GPIOIndex gpioIndex) {
    // get the divider state based on the IoIndex
    // pass the divider state to the DivisionService::tick method
    // if the return value of tick is TRUE
    // make the PORTB bit associated with the pin this DivisionState is focused on 1
    if (DivisionService::tick(state.divisionStateAtIndex(gpioIndex))) {
        PORTB |= (1 << registerLocation);
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
        DisplayService::printLine(state.getBpm(), BPM);
    }
    // get button read pin value and pass it to the state machine in ButtonState
    state.buttonStateAtIndex(GPIOIndex::ONE).updateState(ButtonService::readPin(7));
    state.buttonStateAtIndex(GPIOIndex::TWO).updateState(ButtonService::readPin(6));
    state.buttonStateAtIndex(GPIOIndex::THREE).updateState(ButtonService::readPin(5));
    state.buttonStateAtIndex(GPIOIndex::FOUR).updateState(ButtonService::readPin(4));
}

