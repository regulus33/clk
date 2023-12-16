//
// Created by zachary rowden on 15.12.2023.
//

#ifndef CLK_KNOB_H
#define CLK_KNOB_H

#include <Arduino.h>

#define POT_DEADZONE 20
#define LAST_ADC_VALUE 120


class Knob {
public:
    static void setup() {
        init_ADC();
        last_adc_value = LAST_ADC_VALUE;
        DEBUG_PRINTLN("[KNOB][SETUP]");
    }

    static uint16_t get_value() {
        ADCSRA |= (1 << ADSC); // Start ADC conversion
        while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
        /* TODO: may casue problems down from uint16_t */
        uint16_t adc_val = ADCL | (ADCH << 8); // Read ADC value
        // If ADC value changed significantly (i.e. not jitter)
        if (abs(adc_val - last_adc_value) > POT_DEADZONE) {
            DEBUG_PRINT("[KNOB][ADC_VALUES] - ");
            DEBUG_PRINT_VAR(adc_val);
            DEBUG_PRINT(" and ");
            DEBUG_PRINTLN_VAR(adc_val);
            last_adc_value = adc_val;
            return adc_val;
        }
        return last_adc_value;
    }

private:
    static uint16_t last_adc_value;  // Static variable

    static void init_ADC() {
        // TODO: is this necessary?
        pinMode(A0, INPUT);

        // Initialize ADC for A0
        ADMUX |= (1 << REFS0); // Set reference voltage to AVCC
        ADMUX &= ~(1 << MUX0); // Clear MUX0 to select channel 0 (A0)
        ADCSRA |= (1 << ADEN); // Enable ADC
    }
};

uint16_t Knob::last_adc_value = LAST_ADC_VALUE;  // Initialize static variable



#endif //CLK_KNOB_H
