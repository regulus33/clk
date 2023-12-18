#include "Arduino.h"
#include "division.h"
#include "clock_manager.h"
#include "timer_manager.h"
#include "oled_display.h"
#include "button.h"

#define INITIAL_INTERVAL 1000
/* Global instances */
ProgramState state;
ClockManager clockManager;

//TODO: pulse received should be in ProgramState
void pulse_callback() { state.set_pulse_received(1); };

Button buttons[4] = {
        Button(7),
        Button(6),
        Button(5),
        Button(4),
};

void setup() {
    DEBUG_SETUP;
    OledDisplay::setup();
    clockManager.setup();
    Knob::setup();
    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].setup();
    }
    TimerManager::setup(INITIAL_INTERVAL, pulse_callback);
    Division::setup();
    state.set_bpm(120);
    DEBUG_MEMPRINT;
}

void loop() {
    if (state.get_pulse_received() == 1) {
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3));

        if (Division::tick(
                state.get_div1_steps(),
                state.get_div1_index_steps(),
                state.get_div1_index_end_of_steps()
        )) {
            DEBUG_PRINTLN("tick");
            PORTB |= (1 << PORTB0);
        }
        state.set_pulse_received(0);
    }

    buttons[0].update();
    buttons[1].update();
    buttons[2].update();
    buttons[3].update();

    state.set_bpm(TimerManager::convert_adc_read_to_bpm(Knob::get_value()));
    if (state.bpm_changed() || state.ppqn_changed()) {
        DEBUG_PRINTLN("[MAIN][LOOP][BPM_OR_PPQN_CHANGED]");
        uint16_t timer_interval = TimerManager::get_timer_interval_microseconds(
                state.get_bpm(),
                state.get_pqn()
        );
        TimerManager::update_timer1_interval(timer_interval);
        OledDisplay::print_line(state.get_bpm(), BPM);
    }
}

