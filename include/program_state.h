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
    /* end TimerManager */

private:

    /* TimerManager */
    uint8_t bpm = 1;
    uint8_t last_bpm = 1;
    uint8_t ppqn = 24;
    uint8_t last_ppqn = 24;
    /* end TimerManager */
};


#endif //CLK_PROGRAM_STATE_H
