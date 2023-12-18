//
// Created by zachary rowden on 14.12.2023.
//

#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H
#define DIV1_PIN 8
#define DIV2_PIN 9
#define DIV3_PIN 10
#define DIV4_PIN 11

#include <stdint-gcc.h>

class ProgramState {
public:
    ProgramState() {}
/* TimerManager */
    void set_bpm(uint8_t new_bpm) {
        last_bpm = bpm;
        bpm = new_bpm;
    }

    uint8_t get_bpm() const {
        return bpm;
    }

    bool bpm_changed() const {
        return bpm != last_bpm;
    }

    void set_ppqn(uint8_t new_ppqn) {
        last_bpm = ppqn;
        ppqn = new_ppqn;
    }

    uint8_t get_pqn() const {
        return ppqn;
    }

    bool ppqn_changed() const {
        return ppqn != last_ppqn;
    }
    /* END TimerManager */

    /* Pulse */
    void set_pulse_received(uint8_t p) {
        pulse_received = p;
    }

    uint8_t get_pulse_received() {
        return pulse_received;
    }
    /* END Pulse */

    /* Division */
    uint16_t& get_div1_steps() {
        return div1_steps;
    }

    void set_div1_steps(uint16_t steps) {
        div1_steps = steps;
    }

    volatile uint8_t& get_div1_index_end_of_steps() {
        return div1_index_end_of_steps;
    }

    void set_div1_index_end_of_steps(uint8_t index) {
        div1_index_end_of_steps = index;
    }

    volatile uint8_t& get_div1_index_steps() {
        return div1_index_steps;
    }

    volatile uint8_t set_div1_index_steps(uint8_t index) {
        return div1_index_end_of_steps = index;
    }

    uint8_t get_div1_pin() {
        return div1_pin;
    }

//    uint16_t div1_steps = 0b0000000000000001;
//    volatile uint8_t div1_index_end_of_steps = 2;
//    volatile uint8_t div1_index_steps = 0;
//    const uint8_t div1_pin = DIV1_PIN;
    /* END Division */


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

    /* Division */
    uint16_t div1_steps = 0b0000000000000001;
    volatile uint8_t div1_index_end_of_steps = 7;
    volatile uint8_t div1_index_steps = 0;
    const uint8_t div1_pin = DIV1_PIN;

    uint16_t div2_steps = 0b0000000000000001;
    volatile uint8_t div2_index_end_of_steps = 2;
    volatile uint8_t div2_index_steps = 0;
    const uint8_t div2_pin = DIV2_PIN;

    uint16_t div3_steps = 0b0000000000000001;
    volatile uint8_t div3_index_end_of_steps = 2;
    volatile uint8_t div3_index_steps = 0;
    const uint8_t div3_pin = DIV3_PIN;

    uint16_t div4_steps = 0b0000000000000001;
    volatile uint8_t div4_index_end_of_steps = 2;
    volatile uint8_t div4_index_steps = 0;
    const uint8_t div4_pin = DIV4_PIN;
    /* END Division */
};


#endif //CLK_PROGRAM_STATE_H
