#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H

#include "development/debug_utils.h"
#include <stdint-gcc.h>
#include "division_state.h"
#include "button_state.h"

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
    const uint8_t ppqn = 24;

    /* PULSE ISR-FLAG */
    uint8_t pulseReceived = 0;

    /* DIVISION | OUTPUT */
    DivisionState dividers[4]; // Array of 4 dividers
    ButtonState buttons[4]; // Array of 4 dividers

    /* CONNECTED TO GLOBAL CALLBACK */
    ClockMode clockMode = ClockMode::Internal;

public:
    // TODO: this kind of sucks... Ideally this would be empty until explicityl updated in main.setup()
    ProgramState() : dividers{
            {IOIndex::ONE, INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 1
            {IOIndex::TWO, INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 2
            {IOIndex::THREE, INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 3
            {IOIndex::FOUR, INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}  // Divider 4
    }, buttons{
            {IOIndex::ONE},
            {IOIndex::TWO},
            {IOIndex::THREE},
            {IOIndex::FOUR}
    } {}

    /* CLOCK Methods */
    void setBpm(uint8_t newBpm) {
        lastBpm = bpm;
        bpm = newBpm;
    }

    uint8_t getBpm() const { return bpm; }

    bool bpmChanged() const { return bpm != lastBpm; }

    // TODO: this is a static number ppqn is about the appearance of bpm on the screen... constantize it
    uint8_t getPpqn() const { return ppqn; }

    /* PULSE ISR-FLAG Methods */
    void setPulseReceived(uint8_t p) { pulseReceived = p; }

    uint8_t getPulseReceived() const { return pulseReceived; }

    // Accessors for Divider States
    DivisionState &getDivider(uint8_t index) {
        if (index >= MAX_DIVIDERS) {
            DEBUG_PRINTLN("[PROGRAM_STATE] - DivState index out of bounds");
        }
        return dividers[index];
    }

    ButtonState &getButton(uint8_t index) {
        if(index >= MAX_DIVIDERS) {
            DEBUG_PRINTLN("[PROGRAM_STATE] - ButtonState index out of bounds");
        }
        return buttons[index];
    }

    void setButtonTriggeredCallbacks(
            DivisionModeChangeCallback divisionModeChangeCallback,
            DivisionChangeCallback divisionChangeCallback,
            ClockModeChangeCallback clockModeChangeCallback
            ) {
          for(auto & button : buttons) {
              button.divisionChangeCallback = divisionChangeCallback;
              button.divisionModeChangeCallback = divisionModeChangeCallback;
              button.clockModeChangeCallback = clockModeChangeCallback;
          }
    }

    void setClockMode(ClockMode newclockMode) {
        clockMode = newclockMode;
    }

    // TODO: check clock mode where applicable to decide internal vs. external clock source
    ClockMode getClockMode() {
        return clockMode;
    }
};

#endif //CLK_PROGRAM_STATE_H
