#ifndef CLK_DIVISION_SERVICE_H
#define CLK_DIVISION_SERVICE_H
constexpr uint8_t MAX_STEP_INDEX = 16;

#include "state/program_state.h"
#include "state/division_state.h"
#include "development/debug_utils.h"

static const uint8_t endOfSteps[MAX_STEP_INDEX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

class DivisionService {
    enum Mode {
        Divider,
        RandomStep
    };
public:
    static void setup() {
        pinMode(DIV1_PIN, OUTPUT);
        pinMode(DIV2_PIN, OUTPUT);
        pinMode(DIV3_PIN, OUTPUT);
        pinMode(DIV4_PIN, OUTPUT);
    }

    static uint8_t tick(DivisionState &dividerState) {
        /*  if indexSteps = 3; then 0000000000001000 */
        uint16_t mask = 1 << dividerState.indexSteps;
        uint8_t tick = (dividerState.steps & mask) >> dividerState.indexSteps;
        dividerState.indexSteps = (dividerState.indexSteps + 1) % endOfSteps[dividerState.indexEndOfSteps];

        return tick;
    }

    static uint8_t getDivisionPrint(DivisionState& divisionState) {
        return divisionState.indexEndOfSteps + 2;
    }

    static uint8_t incrementIndexEndOfSteps(DivisionState& divisionState) {
        if(divisionState.mode != DivisionMode::Divider) {
            DEBUG_PRINTLN("[WARNING][WARNING] incrementIndexEndOfSteps() called without being in Divider Mode!!!!");
        } else {
            DEBUG_PRINT("[DIVISION_STATE][INCREMENT] - current indexEndOfSteps");
            DEBUG_PRINTLN_VAR(divisionState.indexEndOfSteps);
            // the division is just the indexEndOfSteps - 1, so you can print the number before it to show it on screen
            divisionState.indexEndOfSteps = (divisionState.indexEndOfSteps + 1) % MAX_STEP_INDEX;
            return getDivisionPrint(divisionState);
        }
    }

};

#endif //CLK_DIVISION_SERVICE_H
