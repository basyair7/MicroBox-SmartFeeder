/**
 * @file variable.h
 * @version 1.0.1
 * @brief This file contains the pin definitions and configuration constants for the project.
 *        It also includes the feeding schedule for the automatic feeder.
 * @date 2024
 * @author basyair7
 */

#pragma once

#include <Arduino.h>
#include "config.h"

// PIN I/O COMPONENTS
#define BOOTBUTTON          18  ///< Pin for the boot button
#define LEDINDIKATOR        BUILTIN_LED  ///< Built-in LED indicator pin
#define PIN_RELAY           13  ///< Pin for the relay module
inline const uint16_t PINOUT_STEPPER[2] = {25, 26};  ///< Pins for the stepper motor control

// PIN ANALOG INPUT SENSOR
#define PIN_PH_SENSOR       35  ///< Pin for the pH sensor
#define PIN_WATER_TURBIDITY 34  ///< Pin for the water turbidity sensor

// PIN I/O ULTRASONIC SENSOR
#define PIN_TRIGGER         14  ///< Pin for the ultrasonic sensor trigger
#define PIN_ECHO            12  ///< Pin for the ultrasonic sensor echo

// Feeding schedule for the automatic feeder
inline const String FeedingSchedule[3] = {"7:00:00", "12:00:00", "17:00:00"};  ///< Feeding times

// Constants for stepper motor control
#define DEGRESS_STEPPER 360  ///< Stepper motor degrees per step
#define SPEED_STEPPER   1000  ///< Stepper motor speed

// EEPROM address definitions
// Do not replace these addresses as they are critical for EEPROM operations
#define ADDR_EEPROM_WIFI_MODE       0x04  ///< EEPROM address for storing Wi-Fi mode
#define ADDR_EEPROM_AUTO_CONTROL    0x08  ///< EEPROM address for storing auto control settings
