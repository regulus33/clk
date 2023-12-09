//
// Created by zachary rowden on 09.12.2023.
//

#ifndef CLK_BUTTON_MANAGER_H
#define CLK_BUTTON_MANAGER_H
#include <Arduino.h>
#include "button.h"
#define BUTTONS_COUNT 4
#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6
#define BUTTON4 7
class ButtonManager {
public:
    static volatile bool interruptOccurred;
private:
    Button buttons[4] = {
            Button(BUTTON1),
            Button(BUTTON2),
            Button(BUTTON3),
            Button(BUTTON4)
    };


ButtonManager() {
    interruptOccurred = false;
}

void setup() {

}

void handleInterrupt() {
    interruptOccurred = true;
}

void update() {
    if (interruptOccurred) {
       for(int i = 0; i < BUTTONS_COUNT; i++) {
           buttons[i].update();
       }
        interruptOccurred = false;
    }
}

// ISR example
void ISR() {
    ButtonManager::handleInterrupt();
}
};
#endif //CLK_BUTTON_MANAGER_H
