/*! @file variable.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>

// #define RX_TX             3
// #define TX_RX             2

#define TRIG_PIN          5
#define ECHO_PIN          4

inline const int16_t PINOUT_STEPPER[4] = {8, 10, 9, 11};
inline const String ListTime[3]        = {"7:0:0", "12:0:0", "17:0:0"};

#define DEGRESS_STEPPER 360
#define SPEED_STEPPER   10

// don't replace it.. I2C address for nodemcu
#define I2C_MASTER_ADDR 0x08
#define JSONSIZE        200
