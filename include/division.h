//
// Created by zack on 9/28/23.
//
#ifndef CLK_OUTPUT_H
#define CLK_OUTPUT_H
#define MAX_STEP_INDEX 16
#define MAX_STEP 16

const char stepsString[MAX_STEP][3] = {
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "11",
        "12",
        "13",
        "14",
        "15",
        "16",
};


/* NOTE: endOfSteps[currentIndexInEndOfSteps] represents the WRAP AROUND POINT in the array  steps*/


struct Division {
    volatile int currentIndexInEndOfSteps = 2;
    volatile int currentIndexInSteps = 0;


public:
    int steps[MAX_STEP]  = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int endOfSteps[MAX_STEP_INDEX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    const char *incrementDiv() {
        currentIndexInEndOfSteps = (currentIndexInEndOfSteps + 1) % MAX_STEP_INDEX;
        return stepsString[currentIndexInEndOfSteps];
    }

    void setDivision(int division) {
        currentIndexInEndOfSteps = division - 1;
    }

    int tick() {
        noInterrupts();

        int tick = steps[currentIndexInSteps];
        currentIndexInSteps = (currentIndexInSteps + 1) % endOfSteps[currentIndexInEndOfSteps];
        interrupts();
        return tick;
    }
};

#endif //CLK_OUTPUT_H
