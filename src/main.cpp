#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"

#define INITIAL_INTERVAL 1000

ProgramState state;

// 👇 Program state is referenced by all 3️⃣ of these callbacks ☎️
// GLOBAL CALLBACKS
// ☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️☎️
void pulseChangeCallback() {
    state.setPulseReceived(1);
}

// TODO
void divisionModeChangeCallback(DivisionMode divisionMode, IOIndex ioIndex) {
    // TODO use ioIndex to locate the divider in programState
    /// TODO update the DivisionMode of each divider (divisionMode member needs to be made still)
}

void divisionChangeCallback(IOIndex ioIndex) {
    DivisionState &divisionState = state.getDivider(ioIndex);
    uint8_t divisionPrint = DivisionService::incrementIndexEndOfSteps(divisionState);
    DisplayService::printLine(divisionPrint, PrintType::Div);
}

void divisionDisplayCallback(IOIndex ioIndex) {
    DivisionState &divisionState = state.getDivider(ioIndex);

    DisplayService::printLine(
            DivisionService::getDivisionPrint(divisionState),
            PrintType::Div
    );
}

// TODO
void clockModeChangeCallback(ClockMode clockMode) {
    state.setClockMode(clockMode);
}
//////////////////////////////////////////////////////////////////////////////


void setup() {
    DEBUG_SETUP;
    DisplayService::setup();
    KnobService::setup();
    ButtonService::setup();
    TimerManager::setup(INITIAL_INTERVAL, pulseChangeCallback);
    DivisionService::setup();
    state.setBpm(120);
    state.setButtonTriggeredCallbacks(divisionModeChangeCallback, divisionChangeCallback, clockModeChangeCallback);
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

    // NOTE 📝 this is really just tracking if there were changes in the BPM value. state has internal checks for it
    // I am not sure how much of a performance boost we would get if we tracked this with a local var in main. TODO
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

