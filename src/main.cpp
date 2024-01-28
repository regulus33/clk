#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"

#define INITIAL_INTERVAL 1000

ProgramState state;

/**
 * @brief Callback function used to handle the timer interrupt service routine.
 *
 * This callback function is invoked when the pulse changes. It updates the program state
 * to indicate that a clock pulse has been received.
 */
void pulseChangeCallback() {
    state.setPulseReceived(1);
}

// TODO
void divisionModeChangeCallback(DivisionMode divisionMode, IOIndex ioIndex) {
    // TODO use ioIndex to locate the divider in programState
    /// TODO update the DivisionMode of each divider (divisionMode member needs to be made still)
}

/**
 * @brief Callback function for division change.
 *
 * This function is called when there is a change in the division for a specific IO index. It increments the indexEndOfSteps
 * value of the DivisionState and prints the updated value on the display.
 *
 * @param ioIndex The IO index for which the division is being changed.
 */
void divisionChangeCallback(IOIndex ioIndex) {
    DivisionState &divisionState = state.getDivider(ioIndex);
    uint8_t divisionPrint = DivisionService::incrementIndexEndOfSteps(divisionState);
    DisplayService::printLine(divisionPrint, PrintType::Div, ioIndex);
}

/**
 * @brief Callback function for displaying division value on the screen.
 *
 * This function is called to display the current division value on the screen.
 *
 * @param ioIndex The index of the input/output device.
 */
void divisionDisplayCallback(IOIndex ioIndex) {
    DivisionState &divisionState = state.getDivider(ioIndex);

    DisplayService::printLine(
            DivisionService::getDivisionPrint(divisionState),
            PrintType::Div,
            ioIndex
    );
}


// TODO
void clockModeChangeCallback(ClockMode clockMode) {
    state.setClockMode(clockMode);
}

void setup() {
    DEBUG_SETUP;
    DisplayService::setup();
    KnobService::setup();
    ButtonService::setup();
    TimerManager::setup(INITIAL_INTERVAL, pulseChangeCallback);
    DivisionService::setup();
    state.setBpm(120);
    state.setButtonTriggeredCallbacks(
            divisionModeChangeCallback,
            divisionChangeCallback,
            clockModeChangeCallback,
            divisionDisplayCallback
    );
    DEBUG_MEMPRINT;
}

/**
 * @brief This function is the main loop of the program.
 *
 * It performs the following tasks:
 * 1. Clears the bits in the PORTB register.
 * 2. Checks if a pulse has been received. If so, it updates the PORTB register based on the state of the dividers.
 * 3. Checks if there has been a change in the BPM value. If so, it updates the timer interval and prints the BPM value on the display.
 * 4. Updates the state of the buttons based on the pin readings.
 *
 * @note The function relies on the ProgramState object named 'state' to access and update the program state.
 * @note The 'tick' function is defined in the DivisionService class and is used to calculate the tick value of a divider.
 * @note The 'convertAdcReadToBpm' function is defined in the TimerManager class and is used to convert the ADC read value to the corresponding BPM value.
 * @note The 'getValue' function is defined in the KnobService class and is used to read the value of the knob.
 * @note The 'getTimerIntervalMicroseconds' function is defined in the TimerManager class and is used to calculate the timer interval in microseconds.
 * @note The 'updateTimer1Interval' function is defined in the TimerManager class and is used to update the Timer1 interval.
 * @note The 'printLine' function is defined in the DisplayService class and is used to print a value on the display.
 * @note The 'readPin' function is defined in the ButtonService class and is used to read the state of a pin.
 */
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

    // NOTE 📝 this is really just tracking if there were changes in the BPM value. state has internal checks for it.
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

