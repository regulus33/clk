//
// Created by zachary rowden on 14.12.2023.
//

#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H

#include <stdint-gcc.h>

class ProgramState {
public:
    ProgramState() {}
/* TimerManager */
    // BPM
    void set_bpm(uint8_t new_bpm) { last_bpm = bpm; bpm = new_bpm;}
    uint8_t get_bpm() const { return bpm;}
    bool bpm_changed() const { return bpm != last_bpm;}
    // PPQN
    void set_ppqn(uint8_t new_ppqn) { last_bpm = ppqn; ppqn = new_ppqn;}
    uint8_t get_pqn() const { return ppqn;}
    bool ppqn_changed() const { return ppqn != last_ppqn;}
    /* END TimerManager */

    /* PULSE */
    void set_pulse_received(uint8_t p) {
        pulse_received = p;
    }

    uint8_t get_pulse_received() {
        return pulse_received;
    }
    /* END PULSE */

private:

    /* TimerManager */
    uint8_t bpm = 1;
    uint8_t last_bpm = 1;
    uint8_t ppqn = 24;
    uint8_t last_ppqn = 24;
    /* END TimerManager */

    /* PULSE */
    uint8_t pulse_received = 0;
    /* END PULSE */
};


#endif //CLK_PROGRAM_STATE_H
