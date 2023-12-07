//
// Created by zack on 9/28/23.
//
#ifndef CLK_OUTPUT_H
#define CLK_OUTPUT_H
#define MAX_STEP_INDEX 16
#define MAX_STEP 16

const int stepString[MAX_STEP] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
};


/* NOTE: endOfSteps[currentIndexInEndOfSteps] represents the WRAP AROUND POINT in the array  steps*/


struct Division {
    volatile int currentIndexInEndOfSteps = 2;
    volatile int currentIndexInSteps = 0;


public:
    uint16_t steps = 0b0000000000000001;
    int endOfSteps[MAX_STEP_INDEX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    const int incrementDiv() {
        currentIndexInEndOfSteps = (currentIndexInEndOfSteps + 1) % MAX_STEP_INDEX;
        return stepString[currentIndexInEndOfSteps];
    }

    void setDivision(int division) {
        currentIndexInEndOfSteps = division - 1;
    }

    int tick() {
//        noInterrupts();
        uint16_t mask = 1 << currentIndexInSteps; // Create a mask for the bit at the given index
        int tick = (steps & mask) >> currentIndexInSteps;
        // TODO: modulo might be kind of expensive, look into alternatives?
        currentIndexInSteps = (currentIndexInSteps + 1) % endOfSteps[currentIndexInEndOfSteps];
//        interrupts();
        return tick;
    }
};

#endif //CLK_OUTPUT_H
