#ifndef CLK_DIVISION_STATE_H
#define CLK_DIVISION_STATE_H

#include "gpio_index.h"
#include "division_mode.h"

#define INIT_DIV_STEPS 0b0000000000000001
#define INIT_DIV_END_STEPS 2
#define INIT_INDEX_STEPS 0

struct DivisionState {
    uint16_t steps; // 1️⃣0️⃣a base 2 16-bit integer that represents a pattern of triggers. applies to ALL modes
    uint8_t indexEndOfSteps; // 🏁 applies to ALL modes but represents ➗when in DivisionMode::Divider
    uint8_t indexSteps; // 📍where we are in the pattern applies to ALL modes
    GPIOIndex ioIndex = GPIOIndex::ONE;
    DivisionMode mode = DivisionMode::Divider;

    DivisionState(
            GPIOIndex ioIndex,
            uint16_t initSteps = INIT_DIV_STEPS,
            uint8_t initIndexEnd = INIT_DIV_END_STEPS,
            uint8_t initIndexSteps = INIT_INDEX_STEPS)
            : ioIndex(ioIndex),
              steps(initSteps),
              indexEndOfSteps(initIndexEnd),
              indexSteps(initIndexSteps
              ) {}

      // returns a display 📿📺string for the screen to show the current division value

};

#endif //CLK_DIVISION_STATE_H
