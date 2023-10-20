//
// Created by zack on 9/28/23.
//

#ifndef CLK_DIVISION_H
#define CLK_DIVISION_H
//
// Created by zack on 9/15/23.
//
const int maxDivision = 10;
const char divisions[10][12] = {
        "24 PPQN | 1",
        "12 PPQN | 2",
        "8 PPQN | 3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "2 PPQN",
        "1 PPQN"

};


const int divs[10] = {1, 2, 3, 4, 5, 6, 7, 8, 12, 24};

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
