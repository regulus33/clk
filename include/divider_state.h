#ifndef CLK_DIVIDER_STATE_H
#define CLK_DIVIDER_STATE_H

constexpr uint16_t INIT_DIV_STEPS = 0b0000000000000001;
constexpr uint8_t INIT_DIV_END_STEPS = 2;
constexpr uint8_t INIT_INDEX_STEPS = 0;

struct DividerState {
    uint16_t steps;
    uint8_t indexEndOfSteps;
    uint8_t indexSteps;

    DividerState(uint16_t initSteps = INIT_DIV_STEPS,
                 uint8_t initIndexEnd = INIT_DIV_END_STEPS,
                 uint8_t initIndexSteps = INIT_INDEX_STEPS)
            : steps(initSteps),
              indexEndOfSteps(initIndexEnd),
              indexSteps(initIndexSteps) {}
};
#endif //CLK_DIVIDER_STATE_H
