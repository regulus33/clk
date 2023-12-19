#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H
#include "development/debug_utils.h"
#include <stdint-gcc.h>
#include "divider_state.h"


constexpr uint8_t DIV1_PIN = 8;
constexpr uint8_t DIV2_PIN = 9;
constexpr uint8_t DIV3_PIN = 10;
constexpr uint8_t DIV4_PIN = 11;
constexpr uint8_t MAX_DIVIDERS = 4;

class ProgramState {
private:
    /* CLOCK */
    uint8_t bpm = 1;
    uint8_t lastBpm = 1;
    uint8_t ppqn = 24;
    uint8_t lastPpqn = 24;

    /* PULSE ISR-FLAG */
    uint8_t pulseReceived = 0;

    /* DIVISION | OUTPUT */
    DividerState dividers[4]; // Array of 4 dividers

public:
    ProgramState() : dividers{
            {INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 1
            {INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 2
            {INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 3
            {INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}  // Divider 4
    } {}

    /* CLOCK Methods */
    void setBpm(uint8_t newBpm) {
        lastBpm = bpm;
        bpm = newBpm;
    }

    uint8_t getBpm() const { return bpm; }

    bool bpmChanged() const { return bpm != lastBpm; }

    void setPpqn(uint8_t newPpqn) {
        lastBpm = ppqn;
        ppqn = newPpqn;
    }

    uint8_t getPpqn() const { return ppqn; }

    bool ppqnChanged() const { return ppqn != lastPpqn; }

    /* PULSE ISR-FLAG Methods */
    void setPulseReceived(uint8_t p) { pulseReceived = p; }

    uint8_t getPulseReceived() const { return pulseReceived; }

    // Accessors for Divider States
    DividerState &getDivider(uint8_t index) {
        if (index >= MAX_DIVIDERS) {
            DEBUG_PRINTLN("[PROGRAM_STATE] - Div index out of bounds");
        }
        return dividers[index];
    }
};

#endif //CLK_PROGRAM_STATE_H
