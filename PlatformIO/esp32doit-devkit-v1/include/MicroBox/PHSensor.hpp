/**
 *  @file PHSensor.hpp
 *  @version 1.0.1
 *  @author 
 *  basyair7
 *  @date 
 *  2024
 * 
 *  @copyright
 *  Copyright (C) 2024, basyair7
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <Arduino.h>
#include "config.h"

/**
 * @class PHSensorClass
 * @brief Class for handling pH sensor operations.
 */
class PHSensorClass {
    public:
        uint16_t ADC_value; // ADC value read from the pH sensor
        float voltage;      // Voltage calculated from the ADC value
        float PH_value;     // Calculated pH value

    public:
        /**
         * @brief Initializes the pH sensor pin.
         * @param pin The pin to which the pH sensor is connected.
         */
        void setup(uint8_t pin) {
            this->_pinout = pin;
            pinMode(pin, INPUT);
        }
        
        /**
         * @brief Reads the sensor value and calculates the pH.
         */
        void run() {
            // Read the ADC value from the sensor.
            this->ADC_value = analogRead(this->_pinout);

            // Calculate the voltage
            this->voltage = this->ADC_value * (VOLTAGE_REF / ADC_RESOLUTION);
            // Calculate the pH step value
            float PH_Steps = (PH4 - PH7) / 3;

            // Calculate the pH value based on voltage
            this->PH_value = 7.0 + ((PH7 - this->voltage) / PH_Steps); 
        }

    private:
        uint8_t _pinout;
};
