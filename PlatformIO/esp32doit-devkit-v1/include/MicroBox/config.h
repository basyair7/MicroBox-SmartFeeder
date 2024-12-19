/**
 * @file config.h
 * @version 1.0.1
 * @brief Configuration file for defining sensors, relay, and stepper driver module.
 * @date 2024
 * @author basyair7
 * 
 * @copyright
 * Copyright (C) 2024, basyair7
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#if defined(ESP32)
    #define VOLTAGE_REF         3.25  ///< Voltage reference for ESP32
    #define ADC_RESOLUTION      4095.0 ///< ADC resolution for ESP32
#elif defined(ESP8266)
    #define VOLTAGE_REF         3.25  ///< Voltage reference for ESP8266
    #define ADC_RESOLUTION      1023.0 ///< ADC resolution for ESP8266
#else
    #define VOLTAGE_REF         5.0   ///< Default voltage reference for other platforms
    #define ADC_RESOLUTION      1023.0 ///< Default ADC resolution for other platforms
#endif

// Constants for Water Turbidity sensor calibration.
#define ADC_CLEAN_WATER 2047    ///< ADC value for clean water (0 NTU)
#define ADC_DIRTY_WATER 1024    ///< ADC value for moderately turbid water (50 NTU)
#define NTU_CLEAN_WATER 0       ///< NTU value for clean water
#define NTU_DIRTY_WATER 50      ///< NTU value for moderately turbid water

// Constants for pH sensor calibration.
#define PH7 2.55    ///< Voltage value for pH 7 (neutral)
#define PH4 3.1     ///< Voltage value for pH 4 (acidic)

// Constants for Ultrasonic Parameters
#define EMPTY 18        // cm
#define FULL  5         // cm

// Constants for Relay Optocoupler
#define ON  0x0
#define OFF 0x1

// Constants for Stepper driver
#define STEPS       300 
#define SETSPEED    1000
