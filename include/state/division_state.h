#ifndef CLK_DIVISION_STATE_H
#define CLK_DIVISION_STATE_H

#include "io_index.h"
#include "division_mode.h"

constexpr uint16_t INIT_DIV_STEPS = 0b0000000000000001;
constexpr uint8_t INIT_DIV_END_STEPS = 2;
constexpr uint8_t INIT_INDEX_STEPS = 0;

struct DivisionState {
    uint16_t steps; // 1️⃣0️⃣a base 2 16-bit integer that represents a pattern of triggers. applies to ALL modes
    uint8_t indexEndOfSteps; // 🏁 applies to ALL modes but represents ➗when in DivisionMode::Divider
    uint8_t indexSteps; // 📍where we are in the pattern applies to ALL modes
    IOIndex ioIndex = IOIndex::ONE;
    DivisionMode mode = DivisionMode::Divider;

    DivisionState(
            IOIndex ioIndex,
            uint16_t initSteps = INIT_DIV_STEPS,
            uint8_t initIndexEnd = INIT_DIV_END_STEPS,
            uint8_t initIndexSteps = INIT_INDEX_STEPS)
            : ioIndex(ioIndex),
              steps(initSteps),
              indexEndOfSteps(initIndexEnd),
              indexSteps(initIndexSteps
              ) {}

      // returns a display 📿📺string for the screen to show the current division valye
    uint8_t incrementDivision() {
        if(mode != DivisionMode::Divider) {
            DEBUG_PRINTLN("[WARNING][WARNING] incrementDivision() called without being in Divider Mode!!!!");
        } else {
            // the division is just the indexEndOfSteps - 1, so you can print the number before it to show it on screen
            indexEndOfSteps = (indexEndOfSteps + 1) % MAX_STEP_INDEX;
            return  indexEndOfSteps - 1;
        }
    }
};

#endif //CLK_DIVISION_STATE_H
