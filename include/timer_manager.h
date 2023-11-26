#ifndef MY_CLOCK_DIVIDER_H
#define MY_CLOCK_DIVIDER_H
#define POT_DEADZONE 20
#define LAST_ADC_VALUE 120

#include <TimerOne.h>
#include <Arduino.h>

#include "clock_manager.h"
#include "ram_service.h"

class TimerManager {
public:
    long intervalMicroSeconds;
    unsigned int bpm;
    unsigned int lastAdcValue = LAST_ADC_VALUE;
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
        // If ADC value changed significantly (i.e. not jitter)
        if(adcValue - lastAdcValue > POT_DEADZONE) {
            // Convert ADC value to BPM
            bpm = map(adcValue, 0, 1023, 180, 50);
            // Change timer frequency
            Timer1.setPeriod(calculateIntervalMicroSecs());
            lastAdcValue = adcValue;
        }

    }



    // Calculate interval in microseconds based on BPM and PPQN
    long calculateIntervalMicroSecs() {
        return 60L * 1000 * 1000 / bpm / ppqn;
    }
};

#endif // MY_CLOCK_DIVIDER_H
