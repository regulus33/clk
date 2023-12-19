#ifndef CLK_KNOB_SERVICE_H
#define CLK_KNOB_SERVICE_H
constexpr  uint16_t POT_DEADZONE = 20;
constexpr uint16_t LAST_ADC_VALUE = 120;
#include <Arduino.h>
#include "development/debug_utils.h"

class KnobService {
public:
    static void setup() {
        initAdc();
        last_adc_value = LAST_ADC_VALUE;
        DEBUG_PRINTLN("[KNOB][SETUP]");
    }

#ifdef TEST_BUILD
    static uint16_t getValue(uint16_t mockRead) {
# else
    static uint16_t getValue() {
#endif
        ADCSRA |= (1 << ADSC); // Start ADC conversion
        while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
#ifdef TEST_BUILD
        uint16_t adcVal = mockRead;
#else
        uint16_t adcVal = ADCL | (ADCH << 8); // Read ADC value
#endif
        // If ADC value changed significantly (i.e. not jitter)
        if (abs(adcVal - last_adc_value) > POT_DEADZONE) {
            DEBUG_PRINT("[KNOB][ADC_VALUES] - ");
            DEBUG_PRINT_VAR(adcVal);
            DEBUG_PRINT(" and ");
            DEBUG_PRINTLN_VAR(adcVal);
            last_adc_value = adcVal;
            return adcVal;
        }
        return last_adc_value;
    }

private:
    static uint16_t last_adc_value;  // Static variable

    static void initAdc() {
        // Initialize ADC for A0
        ADMUX |= (1 << REFS0); // Set reference voltage to AVCC
        ADMUX &= ~(1 << MUX0); // Clear MUX0 to select channel 0 (A0)
        ADCSRA |= (1 << ADEN); // Enable ADC
    }
};

uint16_t KnobService::last_adc_value = LAST_ADC_VALUE;  // Initialize static variable



#endif //CLK_KNOB_SERVICE_H
