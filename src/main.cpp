#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"

#define INITIAL_INTERVAL 1000

// NOTE 📝
// Dividers ➗ and  Buttons connect via 🔘 INDEX
//1️⃣
constexpr uint8_t BUTTON1 = 0;
constexpr uint8_t DIVIDER1 = 0;
////////////////////////////
//2️⃣
constexpr uint8_t BUTTON2 = 1;
constexpr uint8_t DIVIDER2 = 1;
////////////////////////////
//3️⃣
constexpr uint8_t DIVIDER3 = 2;
constexpr uint8_t BUTTON3 = 2;
////////////////////////////
//4️⃣
constexpr uint8_t DIVIDER4 = 3;
constexpr uint8_t BUTTON4 = 3;
////////////////////////////

ProgramState state;
// 👇 Program state is referenced by all 3️⃣ of these callbacks ☎️
// GLOBAL CALLBACKS
//📞📞📞
void pulseChangeCallback() {
    state.setPulseReceived(1);
}
void divisionModeChangeCallback(DivisionMode divisionMode, IOIndex ioIndex) {
    // TODO use ioIndex to locate the divider in programState
    /// TODO update the DivisionMode of each divider (divisionMode member needs to be made still)
}
void divisionChangeCallback(uint8_t division, IOIndex ioIndex) {
    // TODO use ioIndex to locate the divider in programState
    /// TODO update the DivisionMode of each divider (divisionMode member needs to be made still)
}
// TODO: it can't be triggered in HeldDown state as state machine isn't active before reset or power on
void clockModeChangeCallback(ClockMode clockMode) {
    state.setClockMode(clockMode);
}
//📞📞📞



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

        if (DivisionService::tick(state.getDivider(DIVIDER1))) {
            PORTB |= (1 << PORTB0);
        }

        if (DivisionService::tick(state.getDivider(DIVIDER2))) {
            PORTB |= (1 << PORTB1);
        }

        if (DivisionService::tick(state.getDivider(DIVIDER3))) {
            PORTB |= (1 << PORTB2);
        }

        if (DivisionService::tick(state.getDivider(DIVIDER4))) {
            PORTB |= (1 << PORTB3);
        }

        state.setPulseReceived(0);
    }

    state.setBpm(TimerManager::convert_adc_read_to_bpm(KnobService::getValue()));
    if (state.bpmChanged()) {
        uint16_t timer_interval = TimerManager::get_timer_interval_microseconds(
                state.getBpm(),
                state.getPpqn()
        );
        TimerManager::update_timer1_interval(timer_interval);
        DisplayService::printLine(state.getBpm(), BPM);
    }

    // TODO: important! loop is not helping with performance! its just to keep things tidy in development for NOW!!!
//    for (int i = 0; i < 5; i++) {
//        state.getButton(i).updateState(ButtonService::readPin(i + 4));
//    }

    state.getButton(BUTTON1).updateState(
        ButtonService::readPin(7)
        );
    state.getButton(BUTTON2).updateState(
            ButtonService::readPin(6)
    );
    state.getButton(BUTTON3).updateState(
            ButtonService::readPin(5)
    );
    state.getButton(BUTTON4).updateState(
            ButtonService::readPin(4)
    );
}

