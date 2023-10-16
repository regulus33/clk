#ifndef MY_CLOCK_DIVIDER_H
#define MY_CLOCK_DIVIDER_H

#include <TimerOne.h>
#include "clock_manager.h"

class TimerManager {
public:
    long intervalMicroSeconds;
    unsigned int bpm;
    byte ppqn;
    void (*clockPulseInterrupt)();
    ClockManager clockManager;
    explicit TimerManager(float initialBPM = 120, byte initialPPQN = 24, void (*clockPulseInterrupt)() = nullptr):
            intervalMicroSeconds(calculateIntervalMicroSecs()),
            bpm(initialBPM),
            ppqn(initialPPQN),
            clockPulseInterrupt(clockPulseInterrupt)
    {}
    // Timer interrupt for clock pulse (to be implemented)

    // Initialize Timer and ADC
    void begin() {
        Timer1.initialize(intervalMicroSeconds);
        Timer1.attachInterrupt(clockPulseInterrupt);

        // Initialize ADC for A0
        ADMUX |= (1 << REFS0); // Set reference voltage to AVCC
        ADMUX &= ~(1 << MUX0); // Clear MUX0 to select channel 0 (A0)
        ADCSRA |= (1 << ADEN); // Enable ADC
    }

    // Calculate BPM from A0
    void updateBPMFromA0() {
        ADCSRA |= (1 << ADSC); // Start ADC conversion
        while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
        uint16_t adcValue = ADCL | (ADCH << 8); // Read ADC value

        // Convert ADC value to BPM here (you can adjust the conversion logic as needed)
        bpm = map(adcValue, 0, 1023, 50, 200);

        // Update timer period
        Timer1.setPeriod(calculateIntervalMicroSecs());
    }



    // Calculate interval in microseconds based on BPM and PPQN
    long calculateIntervalMicroSecs() {
        return 60L * 1000 * 1000 / bpm / ppqn;
    }
};

#endif // MY_CLOCK_DIVIDER_H
