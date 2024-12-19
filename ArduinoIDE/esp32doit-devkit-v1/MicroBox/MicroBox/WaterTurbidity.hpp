/**
 *  @file WaterTurbidity.hpp
 *  @version 1.0.1
 *  @brief Header file for the Water Turbidity Sensor class.
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
 * @class WaterTurbidityClass
 * @brief Class for handling water turbidity sensor operations.
 */
class WaterTurbidityClass {
    public:
        uint16_t ADC_value;     ///< ADC value read from the turbidity sensor
        float voltage;          ///< Voltage calculated from the ADC value
        float NTU_value;        ///< Calculated NTU value

        /**
         * @brief Initializes the turbidity sensor pin.
         * @param pin The pin to which the turbidity sensor is connected.
         */
        void setup(uint8_t pin) {
            this->_pinout = pin;
            pinMode(pin, INPUT);
        }
        
        /**
         * @brief Reads the sensor value and calculates the NTU (Nephelometric Turbidity Units).
         * @param polynomial Optional flag to use polynomial calculation instead of linear regression.
         * @param a Coefficient for the quadratic term in the polynomial equation.
         * @param b Coefficient for the linear term in the polynomial equation.
         * @param c Constant term in the polynomial equation.
         */
        void run(bool polynomial = false, long a = -2572.2, long b = 8700.5, const long c = 4352.9) {
            // Read the ADC value from the sensor.
            this->ADC_value = analogRead(this->_pinout);

            if (polynomial) {
                // Calculate the voltage
                this->voltage   = (ADC_value / ADC_RESOLUTION) * VOLTAGE_REF;
                // Apply the polynomial equation
                // NTU formula = -ax^2 + bx - c
                this->NTU_value = a * pow(voltage, 2) + b * voltage - c;

                // Ensure NTU is within the valid range
                this->NTU_value = (this->NTU_value < 0) ? 0 : this->NTU_value;
            } else {
                // Calculate NTU using linear regression
                this->NTU_value = NTU_CLEAN_WATER +
                                ((float)(this->ADC_value - ADC_CLEAN_WATER) *
                                (NTU_DIRTY_WATER - NTU_CLEAN_WATER) /
                                (ADC_DIRTY_WATER - ADC_CLEAN_WATER));

                // Ensure NTU is within the valid range
                // this->NTU_value = constrain(this->NTU_value, NTU_CLEAN_WATER, NTU_DIRTY_WATER);
                this->NTU_value = (this->NTU_value < NTU_CLEAN_WATER) ? NTU_CLEAN_WATER : (this->NTU_value > NTU_DIRTY_WATER) ? NTU_DIRTY_WATER : this->NTU_value;
            }
        }

    private:
        uint8_t _pinout;
};
