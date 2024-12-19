/**
 *  @file externprog.h
 *  @version 1.0.0
 *  @author basyair7
 *  @date 2024
*/

#pragma once

#include "WaterTurbidity.hpp"
#include "PHSensor.hpp"
#include "DS3231rtc.hpp"
#include "MyEEPROM.hpp"
#include "MyStepper.hpp"
#include "ProgramWiFi.h"

extern PHSensorClass PHSensor;
extern WaterTurbidityClass WaterTurbidity;
extern DS3231rtc rtcprog;
extern MyEEPROM myeeprom_prog;
extern MyStepper mystepper;

extern unsigned long LastTimeReboot;
extern bool RebootState;
extern float ultrasonic_capacity;