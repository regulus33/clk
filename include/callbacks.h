//
// Created by zachary rowden on 31.01.2024.
//

#ifndef CLK_CALLBACKS_H
#define CLK_CALLBACKS_H
// callbacks.h

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <Arduino.h>
#include "state/gpio_index.h"
#include "state/clock_mode.h"
#include "state/division_mode.h"

typedef void (*DivisionModeChangeCallback)(DivisionMode, GPIOIndex);
typedef void (*DivisionChangeCallback)(GPIOIndex, uint8_t);
typedef void (*ClockModeChangeCallback)(ClockMode);
typedef void (*PulseChangeCallback)();
typedef void (*WriteToPortStub)(GPIOIndex);

#endif // CALLBACKS_H

#endif //CLK_CALLBACKS_H
