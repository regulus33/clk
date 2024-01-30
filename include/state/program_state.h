#ifndef CLK_PROGRAM_STATE_H
#define CLK_PROGRAM_STATE_H

#include "development/debug_utils.h"
#include <stdint-gcc.h>
#include "division_state.h"
#include "button_state.h"

// pin-numbers for divs
#define DIV1_PIN 8
#define DIV2_PIN 9
#define DIV3_PIN 10
#define DIV4_PIN 11
#define MAX_DIVIDERS 4
//
class ProgramState {
private:
    // bpm
    uint8_t bpm = 1;
    // set last bpm, so we don't update screen unless there is a diff between bpm and last bpm
    uint8_t lastBpm = 1;

    // main flag to tell us whether to tick all divisions
    uint8_t pulseReceived = false;

    // array of divider instances
    DivisionState dividers[4];
    // array of button instances
    ButtonState buttons[4];

    // TODO NOT IMPLEMENTED
    ClockMode clockMode = ClockMode::Internal;



public:
    // constructor
    ProgramState() : dividers{
            {GPIOIndex::ONE,   INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 1
            {GPIOIndex::TWO,   INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 2
            {GPIOIndex::THREE, INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}, // Divider 3
            {GPIOIndex::FOUR,  INIT_DIV_STEPS, INIT_DIV_END_STEPS, INIT_INDEX_STEPS}  // Divider 4
    }, buttons{
            {GPIOIndex::ONE},
            {GPIOIndex::TWO},
            {GPIOIndex::THREE},
            {GPIOIndex::FOUR}
    } {}

    // this is called after knob read to update the current bpm
    void setBpm(uint8_t newBpm) {
        // set the last bpm so we can compare it against the next
        lastBpm = bpm;
        // set current bpm
        bpm = newBpm;
    }

    // get the bpm
    uint8_t getBpm() const { return bpm; }

    bool bpmChanged() const { return bpm != lastBpm; }

    /* PULSE ISR-FLAG Methods */
    void setPulseReceived(uint8_t p) { pulseReceived = p; }

    uint8_t getPulseReceived() const { return pulseReceived; }

    // Accessors for Divider States
    DivisionState &divisionStateAtIndex(GPIOIndex index) {
        // error "handling"
        if (index >= MAX_DIVIDERS) {
            DEBUG_PRINTLN("[PROGRAM_STATE] - DivState index out of bounds");
        }
        // return the divisionState reference
        return dividers[index];
    }

    ButtonState &buttonStateAtIndex(GPIOIndex index) {
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
