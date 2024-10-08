/*! @file I2C_Slave.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

extern void I2CSlave_Init(uint8_t address, uint16_t jsonSize);
extern void I2CSlave_getData(int* capacity, bool* auto_state, bool* switch_state);