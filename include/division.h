//
// Created by zack on 9/28/23.
//

#ifndef CLK_DIVISION_H
#define CLK_DIVISION_H
#define MAX_DIV_COUNT 8
//
// Created by zack on 9/15/23.
//
const int maxDivision = MAX_DIV_COUNT;
const char divisions[MAX_DIV_COUNT][13] = {
        "24 PPQN",
        "1 / 2",
        "1 / 3",
        "1 / 4",
        "1 / 5",
        "1 / 6",
        "1 / 7",
        "1 / 8",

};


const int divs[MAX_DIV_COUNT] = {1, 2, 3, 4, 5, 6, 7, 8 };

struct Division {
    int counter = 1;
    int divIndex = 1;

public:
    const char *incrementDiv() {
        divIndex = (divIndex + 1) % maxDivision;
        return divisions[divIndex];
    }

    void setDivision(int division) {
        divIndex = division - 1;
    }

    int tick() {
        int tick = 0;
        if (counter % divs[divIndex] == 0) {
            tick = 1;
        }
        counter = (counter % 8) + 1;
        return tick;
    }

    void advance() {
        counter = (counter % 8) + 1;
    }

    void resetState() {
        counter = 0;
    }
};

#endif //CLK_DIVISION_H
