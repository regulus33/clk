//
// Created by zachary rowden on 09.12.2023.
//

#ifndef CLK_DEBUG_UTILS_H
#define CLK_DEBUG_UTILS_H

#ifdef DEBUG
#define DEBUG_PRINT(str) Serial.print(F(str))
#define DEBUG_PRINTLN(str) Serial.println(F(str))
#define DEBUG_PRINT_VAR(var) Serial.print(var)
#define DEBUG_PRINTLN_VAR(var) Serial.println(var)
#define DEBUG_LN_BR Serial.println(F(""))
#define DEBUG_PRINT_STATE printState(state)
#else
#define DEBUG_PRINT(str)
#define DEBUG_PRINTLN(str)
#define DEBUG_PRINT_VAR(var)
#define DEBUG_PRINTLN_VAR(var)
#define DEBUG_LN_BR
#define DEBUG_PRINT_STATE
#endif

#endif // CLK_DEBUG_UTILS_H

