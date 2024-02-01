//
// Created by zachary rowden on 01.02.2024.
//
#include <Arduino.h>
#include "config.h"
#include "hardware/button_service.h"

#ifndef CLK_SETUP_CONFIGURATION_SERVICE_H
#define CLK_SETUP_CONFIGURATION_SERVICE_H
#define CLOCK_BTN_PIN 7
#define DSPL_BTN_PIN 6
#define CLOCK_BTN_CONFIRMATION "x clk on"
#define DSPL__BTN_CONFIRMATION "scrn-off"


class SetupConfigurationService {
public:
    // Dsbl Dsply -------------------
    static bool userDisabledDisplay() {
        // this means button 1 is pressed
        return !ButtonService::readPin(DSPL_BTN_PIN);
    }

    static void confirmUserDisabledDisplay() {
        DisplayService::drawCharBuffer(DSPL__BTN_CONFIRMATION);
    }
    // Ext clock --------------------
    static bool userEnabledExtClock() {
        // this means button 1 is pressed
        return !ButtonService::readPin(CLOCK_BTN_PIN);
    }

    static void confirmUserEnabledExtClock() {
        DisplayService::drawCharBuffer(CLOCK_BTN_CONFIRMATION);
    }
};

#endif //CLK_SETUP_CONFIGURATION_SERVICE_H