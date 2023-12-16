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
int pulseReceived = 0;
OledDisplay display;
Knob knob = Knob(state);
void pulse_callback() { pulseReceived = 1; };

Button buttons[4] = {
        Button(7),
        Button(6),
        Button(5),
        Button(4),
};

void setup() {
    DEBUG_SETUP;
    display.setup();
    clockManager.setup();
    knob.setup();
    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].setup();
    }
    TimerManager::setup(INITIAL_INTERVAL, pulse_callback);
    state.set_bpm(120);
    DEBUG_MEMPRINT;
}

void loop() {
    if (pulseReceived == 1) {
        DEBUG_PRINTLN("[MAIN][LOOP][PULSE_RECEIVED]");
        clockManager.tick();
        pulseReceived = 0;
    }

    buttons[0].update();
    buttons[1].update();
    buttons[2].update();
    buttons[3].update();

    state.set_bpm(TimerManager::convert_adc_read_to_bpm(knob.get_value()));
    if (state.bpm_changed() || state.ppqn_changed()) {
        DEBUG_PRINTLN("[MAIN][LOOP][BPM_OR_PPQN_CHANGED]");
        uint16_t timer_interval = TimerManager::get_timer_interval_microseconds(
                state.get_bpm(),
                state.get_pqn()
                );
        TimerManager::update_timer1_interval(timer_interval);
        display.printLine(state.get_bpm(), BPM);
    }
}

