/*! @file variable.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>

#define LED_YELLOW        D5
#define LED_RED           D6

#define TRIG_PIN          D7
#define ECHO_PIN          D8

// #define RX_TX             D1
// #define TX_RX             D2

#define SCL_PIN           D1
#define SDA_PIN           D2

#define BOOTBUTTON        D3

#define SPEED_GETDISTANCE 10    // 0.01s
#define EMPTY             18    // cm
#define FULL              5     // cm

// don't replace it.. I2C address for uno
#define I2C_SLAVE_ADDR    0x08 
#define JSONSIZE          200

// don't replace it... EEPROM state blynk mode
#define EEPROM_BLYNK_ADDR 0x4 