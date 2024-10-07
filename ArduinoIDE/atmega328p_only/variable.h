/*! @file variable.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>

#define TRIG_PIN  5
#define ECHO_PIN  4

#define LED_1     3     // red
#define LED_2     2     // green

inline const int16_t PINOUT_STEPPER[4] = {8, 10, 9, 11};
inline String List_Time[3]             = {"7:0:0", "12:30:0", "17:0:0"};

