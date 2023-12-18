#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H
#define DIV1_PIN 8
#define DIV2_PIN 9
#define DIV3_PIN 10
#define DIV4_PIN 11
#define INIT_DIV_STEPS 0b0000000000000001
#define INIT_DIV_END_STEPS 2
#define INIT_INDEX_STEPS 0
#include <stdint-gcc.h>

class ProgramState {
private:
    /* CLOCK-----------------------------*/
    uint8_t bpm = 1;
    uint8_t last_bpm = 1;
    uint8_t ppqn = 24;
    uint8_t last_ppqn = 24;

    /* PULSE ISR-FLAG--------------------*/
    uint8_t pulse_received = 0;

    /* DIVISION | OUTPUT----------------*/
    /* 1------OUTPUT-GETTERS & SETTERS-*/
    uint16_t div1_steps = INIT_DIV_STEPS;
    volatile uint8_t div1_index_end_of_steps = INIT_DIV_END_STEPS;
    volatile uint8_t div1_index_steps = INIT_INDEX_STEPS;
    /* 2------OUTPUT-GETTERS & SETTERS-*/
    uint16_t div2_steps = INIT_DIV_STEPS;
    volatile uint8_t div2_index_end_of_steps = INIT_DIV_END_STEPS;
    volatile uint8_t div2_index_steps = INIT_INDEX_STEPS;
    /* 3------OUTPUT-GETTERS & SETTERS-*/
    uint16_t div3_steps = INIT_DIV_STEPS;
    volatile uint8_t div3_index_end_of_steps = INIT_DIV_END_STEPS;
    volatile uint8_t div3_index_steps = INIT_INDEX_STEPS;
    /* 4------OUTPUT-GETTERS & SETTERS-*/
    uint16_t div4_steps = INIT_DIV_STEPS;
    volatile uint8_t div4_index_end_of_steps = INIT_DIV_END_STEPS;
    volatile uint8_t div4_index_steps = INIT_INDEX_STEPS;
public:
    /* CLOCK-----------------------------*/
    void set_bpm(uint8_t new_bpm) {
        last_bpm = bpm;
        bpm = new_bpm;
    }

    uint8_t get_bpm() const { return bpm; }

    bool bpm_changed() const { return bpm != last_bpm; }

    void set_ppqn(uint8_t new_ppqn) {
        last_bpm = ppqn;
        ppqn = new_ppqn;
    }

    uint8_t get_pqn() const { return ppqn; }

    bool ppqn_changed() const { return ppqn != last_ppqn; }

    /* PULSE ISR-FLAG--------------------*/
    void set_pulse_received(uint8_t p) { pulse_received = p; }

    uint8_t get_pulse_received() { return pulse_received; }

    /* DIVISION | OUTPUT----------------*/
    /* 1------OUTPUT-GETTERS & SETTERS-*/
    uint16_t &get_div1_steps() { return div1_steps; }

    void set_div1_steps(uint16_t steps) { div1_steps = steps; }

    volatile uint8_t &get_div1_index_end_of_steps() { return div1_index_end_of_steps; }

    void set_div1_index_end_of_steps(uint8_t index) { div1_index_end_of_steps = index; }

    volatile uint8_t &get_div1_index_steps() { return div1_index_steps; }

    volatile uint8_t set_div1_index_steps(uint8_t index) { return div1_index_end_of_steps = index; }

    /* 2------OUTPUT-GETTERS & SETTERS-*/
    uint16_t &get_div2_steps() { return div2_steps; }

    void set_div2_steps(uint16_t steps) { div2_steps = steps; }

    volatile uint8_t &get_div2_index_end_of_steps() { return div2_index_end_of_steps; }

    void set_div2_index_end_of_steps(uint8_t index) { div2_index_end_of_steps = index; }

    volatile uint8_t &get_div2_index_steps() { return div2_index_steps; }

    volatile uint8_t set_div2_index_steps(uint8_t index) { return div2_index_end_of_steps = index; }

    /* 3------OUTPUT-GETTERS & SETTERS-*/
    uint16_t &get_div3_steps() { return div3_steps; }

    void set_div3_steps(uint16_t steps) { div3_steps = steps; }

    volatile uint8_t &get_div3_index_end_of_steps() { return div3_index_end_of_steps; }

    void set_div3_index_end_of_steps(uint8_t index) { div3_index_end_of_steps = index; }

    volatile uint8_t &get_div3_index_steps() { return div3_index_steps; }

    volatile uint8_t set_div3_index_steps(uint8_t index) { return div3_index_end_of_steps = index; }

    /* 4------OUTPUT-GETTERS & SETTERS-*/
    uint16_t &get_div4_steps() { return div4_steps; }

    void set_div4_steps(uint16_t steps) { div4_steps = steps; }

    volatile uint8_t &get_div4_index_end_of_steps() { return div4_index_end_of_steps; }

    void set_div4_index_end_of_steps(uint8_t index) { div4_index_end_of_steps = index; }

    volatile uint8_t &get_div4_index_steps() { return div4_index_steps; }

    volatile uint8_t set_div4_index_steps(uint8_t index) { return div4_index_end_of_steps = index; }
};


#endif //CLK_PROGRAM_STATE_H
