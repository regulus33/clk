#ifndef CLK_DIVISION_H
#define CLK_DIVISION_H
constexpr uint8_t MAX_STEP_INDEX = 16;
#include "state/program_state.h"
#include "state/divider_state.h"
#include "development/debug_utils.h"

static const uint8_t endOfSteps[MAX_STEP_INDEX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

class Division {

public:
    static void setup() {
        pinMode(DIV1_PIN, OUTPUT);
        pinMode(DIV2_PIN, OUTPUT);
        pinMode(DIV3_PIN, OUTPUT);
        pinMode(DIV4_PIN, OUTPUT);
    }

    static uint8_t tick(DividerState& dividerState) {
        /*  if indexSteps = 3; then 0000000000001000 */
        uint16_t mask = 1 << dividerState.indexSteps;
        int tick = (dividerState.steps & mask) >> dividerState.indexSteps;
        dividerState.indexSteps = (dividerState.indexSteps + 1) % endOfSteps[dividerState.indexEndOfSteps];
        DEBUG_PRINTLN_VAR(dividerState.indexSteps);
        return tick;
    }

};
#endif //CLK_DIVISION_H
