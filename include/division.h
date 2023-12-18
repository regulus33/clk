#ifndef CLK_DIVISION_H
#define CLK_DIVISION_H
#define MAX_STEP_INDEX 16
#define MAX_STEP 16
#include "program_state.h"
#include "debug_utils.h"

static const uint8_t end_of_steps[MAX_STEP_INDEX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
static const int stepString[MAX_STEP] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};

class Division {
    uint8_t pin;

public:
    explicit Division(uint8_t pin) : pin(pin) {}

    static void setup() {
        pinMode(DIV1_PIN, OUTPUT);
        pinMode(DIV2_PIN, OUTPUT);
        pinMode(DIV3_PIN, OUTPUT);
        pinMode(DIV4_PIN, OUTPUT);
    }

    static int incrementDiv(int &
    currentIndexInEndOfSteps) {
        return (currentIndexInEndOfSteps + 1) % MAX_STEP_INDEX;
    }


    static uint8_t tick(volatile uint16_t &steps, volatile uint8_t &index_steps, volatile uint8_t &index_end_of_steps) {
        uint16_t mask = 1 << index_steps;
        int tick = (steps & mask) >> index_steps;
        index_steps = (index_steps + 1) % end_of_steps[index_end_of_steps];
        DEBUG_PRINTLN_VAR(index_steps);
        return tick;
    }
};
#endif //CLK_DIVISION_H
