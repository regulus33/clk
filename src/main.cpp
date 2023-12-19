#include "Arduino.h"
#include "division.h"
#include "timer_manager.h"
#include "oled_display.h"
#include "button.h"
#include "program_state.h"
#define INITIAL_INTERVAL 1000

ProgramState state;
void pulse_callback() { state.setPulseReceived(1); };
Button buttons[4] = {Button(7),Button(6),Button(5),Button(4) };

void setup() {
    DEBUG_SETUP;
    OledDisplay::setup();
    Knob::setup();
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

    state.setBpm(TimerManager::convert_adc_read_to_bpm(Knob::getValue()));
    if (state.bpmChanged() || state.ppqnChanged()) {
        uint16_t timer_interval = TimerManager::get_timer_interval_microseconds(
                state.getBpm(),
                state.getPpqn()
        );
        TimerManager::update_timer1_interval(timer_interval);
        OledDisplay::printLine(state.getBpm(), BPM);
    }
}

