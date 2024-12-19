/**
 *  @file EEPROM.hpp
 *  @version 1.0.1
 *  @date 2024
 *  @author basyair7
 * 
 *  @brief A wrapper class for EEPROM operations, compatible with ESP and AVR platforms.
 * 
 *  This class provides template-based methods for reading, writing, and saving specific 
 *  program states using the EEPROM memory.
 */

#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "variable.h"

class MyEEPROM {
    public:
        /**
         * @brief Read a value of type T from EEPROM.
         * @tparam T The type of the value to read.
         * @param address The EEPROM address to read from.
         * @param val Reference to store the read value.
         * @return A reference to the read value.
         */
        template <typename T>
        T &get(const int address, T &val) const {
            return EEPROM.get(address, val);
        }

        /**
         * @brief Read a single byte from EEPROM.
         * @param address The EEPROM address to read from.
         * @return The byte read from EEPROM.
         */
        uint8_t read(const int address) const {
            return EEPROM.read(address);
        }

        /**
         * @brief Save a state to EEPROM.
         * @tparam T The type of the state to save.
         * @param address The EEPROM address to write to.
         * @param state the state value to save.
         * @return `true` if the operation succeeded, `false` otherwise.
         */
        template <typename T>
        bool save_state(const int address, const T &state) const {
            EEPROM.put(address, state);
            #if defined(ESP8266) || defined(ESP32)
                return EEPROM.commit();
            #else
                return true;
            #endif
        }

        /**
         * @brief Save the WiFi mode state to EEPROM.
         * @tparam T The type of the state to save.
         * @param state the Blynk mode state to save.
         * @return `true` if the operation succeeded, `false` otherwise.
         */
        template <typename T>
        bool save_wifi_state(const T &state) const {
            return this->save_state(ADDR_EEPROM_WIFI_MODE, state);
        }

        /**
         * @brief Save the Auto Control state to EEPROM.
         * @tparam T The type of the state to save.
         * @param state The watering state to save.
         * @return `true` if the operation succeeded, `false` otherwise.
         */
        template <typename T>
        bool save_state_auto_control(const T &state) const {
            return this->save_state(ADDR_EEPROM_AUTO_CONTROL, state);
        }

        /**
         * @brief Initialize EEPROM for use. Required for ESP platforms.
         * @param size The size of the EEPROM to initialize (ESP only).
         * @return `true` if the operation succeeded (ESP32) or assumed success (ESP8266/AVR)
         */
        bool initialize(int size = 512) const {
            #if defined(ESP32)
                // For ESP32, EEPROM.begin(size) returns a bool
                return EEPROM.begin(size);
            #elif defined(ESP8266)
                // For ESP8266, EEPROM.begin(size) returns void
                EEPROM.begin(size); return true; // Assume success as no error handling is available
            #else
                // For AVR platforms, no initialization is required
                return true;
            #endif
        }

        /**
         * @brief End the EEPROM session (ESP platforms only).
         */
    #if defined(ESP32) || defined(ESP8266)
        bool cleanup() const {
            #if defined(ESP32)
                EEPROM.end(); return true;
            #elif defined(ESP8266)
                return EEPROM.end();
            #endif
        }
    #endif
};
