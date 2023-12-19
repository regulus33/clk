#include "Arduino.h"
#include "division.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#define INITIAL_INTERVAL 1000

ProgramState state;
void pulse_callback() { state.setPulseReceived(1); };
ButtonService buttons[4] = {ButtonService(7), ButtonService(6), ButtonService(5), ButtonService(4) };

void setup() {
    DEBUG_SETUP;
    DisplayService::setup();
    KnobService::setup();
    for (int i = 0; i < BUTTON_COUNT; i++) { buttons[i].setup();}
    TimerManager::setup(INITIAL_INTERVAL, pulse_callback);
    Division::setup();
    state.setBpm(120);
    DEBUG_MEMPRINT;
}

void loop() {
    if (state.getPulseReceived() == 1) {
        /* CLEAR BITS IN PORTB REGISTER */
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));

        if (Division::tick(state.getDivider(0))) {
            PORTB |= (1 << PORTB0);
        }

        if (Division::tick(state.getDivider(1))) {
            PORTB |= (1 << PORTB1);
        }

        if (Division::tick(state.getDivider(2))) {
            PORTB |= (1 << PORTB2);
        }

        if (Division::tick(state.getDivider(3))) {
            PORTB |= (1 << PORTB3);
        }

        state.setPulseReceived(0);
    }

    buttons[0].update();
    buttons[1].update();
    buttons[2].update();
    buttons[3].update();

    state.setBpm(TimerManager::convert_adc_read_to_bpm(KnobService::getValue()));
    if (state.bpmChanged() || state.ppqnChanged()) {
        uint16_t timer_interval = TimerManager::get_timer_interval_microseconds(
                state.getBpm(),
                state.getPpqn()
        );
        TimerManager::update_timer1_interval(timer_interval);
        DisplayService::printLine(state.getBpm(), BPM);
    }
}

