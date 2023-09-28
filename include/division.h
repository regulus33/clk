//
// Created by zack on 9/28/23.
//

#ifndef CLK_DIVISION_H
#define CLK_DIVISION_H
//
// Created by zack on 9/15/23.
//
const int maxDivision = 8;
const char* divisions[] = {
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
};
const int divs[8] = {1,2,3,4,5,6,7,8};

struct Division {
    int counter = 1;
    int divIndex = 0;

public:
    const char* incrementDiv() {
        divIndex = (divIndex + 1) % maxDivision;
        return divisions[divIndex];
    }

    void setDivision(int division) {
        divIndex = division - 1;
    }

    int willTick() {
        return counter % divs[divIndex] == 0;
    }

    int tick() {
        int tick = 0;
        if (counter % divs[divIndex] == 0) {
            tick = 1;
        }
        counter = (counter % 8) + 1;
        return tick;
    }

    void resetState() {
        counter = 0;
    }
};
#endif //CLK_DIVISION_H
