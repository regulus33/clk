#include "Arduino.h"
#include "division_service.h"
#include "timer_manager.h"
#include "hardware/display_service.h"
#include "hardware/button_service.h"
#include "state/program_state.h"
#include "state/division_mode.h"
#include "config.h"
#include "clock_divider.h"

#define INITIAL_INTERVAL 1000
#define CLOCK_BTN_PIN 7
#define CLOCK_BTN_CONFIRMATION "x clk on"
#define EXTERNAL_CLOCK_PIN 2  // Use pin 2 for external clock
// #define EXTERNAL_CLOCK_PIN 3  // Uncomment to use pin 3 instead

// Initialize container for all our program state
ProgramState state;

void pulseChangeCallback() {
    state.setPulseReceived(1);
}

void divisionModeChangeCallback(DivisionMode divisionMode, GPIOIndex ioIndex) {}

void divisionChangeCallback(GPIOIndex ioIndex, uint8_t incrementEndOfSteps) {
    uint8_t divisionPrint;
    DivisionState &divisionState = state.divisionStateAtIndex(ioIndex);
    if (incrementEndOfSteps == true) {
        divisionPrint = DivisionService::incrementIndexEndOfSteps(divisionState);
    } else {
        divisionPrint = DivisionService::getDivisionPrint(divisionState);
    }
    DisplayService::updateOled(divisionPrint, PrintType::Div, ioIndex);
}

void clockModeChangeCallback(ClockMode clockMode) {
    state.setClockMode(clockMode);
}


ClockDivider clockDivider(
        state,
        divisionChangeCallback,
        pulseChangeCallback,
        divisionModeChangeCallback,
        clockModeChangeCallback
);

bool userEnabledExtClock() {
    // this means button 1 is pressed
    return !ButtonService::readPin(CLOCK_BTN_PIN);
}

void setup() {
    // sets up some logging stuff if building with debug flags
    DEBUG_SETUP;
    // sets up pins and I2C stuff for the display
    DisplayService::setup();
    // sets up or own adc logic for faster reads (no HAL used)
    KnobService::setup();
    // Setup pin inputs
    ButtonService::setup();
    //
    if(userEnabledExtClock()) {
        DisplayService::drawCharBuffer(CLOCK_BTN_CONFIRMATION);
        pinMode(EXTERNAL_CLOCK_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(EXTERNAL_CLOCK_PIN), pulseChangeCallback, RISING);
        delay(EXT_CLOCK_MSG_TIME);
    } else {
        // Attach our callback / ISR to the Timer1 library's timer1 service.
        // It will call this function on each clock cycle
        TimerManager::setup(INITIAL_INTERVAL, pulseChangeCallback);
    }

    // set up the pins toward which the OUTPUTS of the program will funnel
    DivisionService::setup();
    // connects callbacks
    clockDivider.setup();
    // Print out how much memory we've used after all this setup process (if in debug mode)
    DEBUG_MEMPRINT;


}

void loop() {
    clockDivider.loop();
}

