#ifndef CLK_RAM_SERVICE_H
#define CLK_RAM_SERVICE_H
#include <Arduino.h>

// very simple RAM printing servicio
class RamService {

public:
    // don't ask!
    static int freeRam() {
        extern int __heap_start, *__brkval;
        int v;
        return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    }

    // calcs and prints total free and percent used
   static void printMemoryUsage() {
        int totalRam = 2048; // Total RAM in bytes for Arduino Uno
        int usedRam = totalRam - freeRam();
        float usedPercentage = (float) usedRam / totalRam * 100;

        Serial.print(F("Memory used: "));
        Serial.print(usedRam);
        Serial.print(F(" bytes ("));
        Serial.print(usedPercentage);
        Serial.println(F("%)"));
    }
};

#endif //CLK_RAM_SERVICE_H