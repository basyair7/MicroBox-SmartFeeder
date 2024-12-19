/**
 *  @file Ultrasonic.hpp
 *  @version 1.0.1
 *  @brief Header file for the Ultrasonic sensor class.
 *  @date 2024
 *  @author basyair7
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

/**
 * @class Ultrasonic
 * @brief Class for handling ultrasonic sensor operations.
 */
class Ultrasonic {
    public:
        /**
         * @brief Constructor for Ultrasonic class.
         * @param TriggerPin The pin number connected to the trigger pin of the ultrasonic sensor.
         * @param EchoPin The pin number connected to the echo pin of the ultrasonic sensor.
         */
        Ultrasonic(uint8_t TriggerPin, uint8_t EchoPin) {
            this->triggerPin = TriggerPin;
            this->echoPin    = EchoPin;
        }

        /**
         * @brief Initializes the ultrasonic sensor pins.
         */
        void begin() {
            pinMode(this->triggerPin, OUTPUT);
            pinMode(this->echoPin, INPUT);
        }

        /**
         * @brief Measures the distance using the ultrasonic sensor.
         * @return The measured distance in centimeters.
         */
        float getDistance() {
            // Send a 10 microseconds pulse to trigger the sensor.
            digitalWrite(this->triggerPin, LOW);
            delayMicroseconds(5);
            digitalWrite(this->triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(this->triggerPin, LOW);

            // Read the echo pin and calculate distance based on the duration of the pulse.
            this->value = pulseIn(this->echoPin, HIGH) * 0.034 / 2;
            return this->value;
        }

    private:
        uint8_t triggerPin; ///< Pin number for the trigger pin.
        uint8_t echoPin;    ///< Pin number for the echo pin.
        float value;
};
