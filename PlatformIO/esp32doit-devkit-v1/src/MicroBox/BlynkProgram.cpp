/**
 *  @file BlynkProgram.cpp
 *  @version 1.0.1
 *  @brief Program for managing Blynk functionalities.
 *  @details This file implements functions for integrating Blynk with the ESP32,
 *           including handling virtual pins and automating tasks via Blynk.
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>
*/

// Include MicroBox Headers
#include "MicroBox/BlynkProgram.h"
#include "MicroBox/ProgramWiFi.h"
#include "MicroBox/MyEEPROM.hpp"
#include "MicroBox/externprog.h"

// Include Blynk Library
#include "envBlynk.h"
#include <BlynkSimpleEsp32.h>

MyEEPROM myeeprom; ///< EEPROM utility module
BlynkTimer Timer;  ///< Blynk timer object for periodic tasks

bool switch_state; ///< State variable for switch control

/**
 * @brief Reboot ESP from Blynk APP command.
 * @param V4 Virtual pin for reboot command.
 * @details When this virtual pin is set, the reboot state is triggered and
 *          stored in the `RebootState` variable.
 */
BLYNK_WRITE(V4) {
    LastTimeReboot = millis();
    RebootState = true;
}

/**
 * @brief Disables Blynk mode or switch WiFi mode on WIFI_AP.
 * @param V3 Virtual pin for Blynk mode disable.
 * @details Saves the state to EEPROM, delays for a stable reboot, and triggers a restart.
 */
BLYNK_WRITE(V3) {
    myeeprom.save_wifi_state(false);
    delay(50);
    LastTimeReboot = millis();
    RebootState = true;
}

/**
 * @brief Manages manual feeder via Blynk.
 * @param V2 Virtual pin for manual feeder.
 * @details Activates or deactivates stepper controlling the feeder system.
 *          Manual feeding is disabled if auto-feeder is active.
 */
BLYNK_WRITE(V2) {
    bool read_auto_control = myeeprom.read(ADDR_EEPROM_AUTO_CONTROL);
    if (read_auto_control) {
        Blynk.virtualWrite(V1, read_auto_control);
        return;
    }

    switch_state = param.asInt() == 1 ? true : false;
}

/**
 * @brief Configures the auto-feeder state via Blynk.
 * @param V1 Virtual pin for auto-feeder configuration.
 */
BLYNK_WRITE(V1) {
    myeeprom.save_state_auto_control(param.asInt() == 1 ? true : false);
}

/**
 * @brief Sends ultrasonic sensor data to Blynk.
 * @details Periodically transmits the capacity level to a virtual pin.
 */
unsigned long LastMillis_SendCapacity = 0;
void sendCapacity(void) {
    if ((unsigned long) (millis() - LastMillis_SendCapacity) >= 100L) {
        LastMillis_SendCapacity = millis();
        Blynk.virtualWrite(V0, ultrasonic_capacity);
    }
}

/**
 * @brief Initializes Blynk and sets up periodic tasks.
 * @details Connects to WiFi, initializes Blynk, and sets up a timer to send sensor data.
 */
void Blynk_setup() {
    if (WiFi.getMode() == WIFI_STA) {
        Serial.println(F("Initialize Blynk"));
        Serial.printf("SSID : %s\nPassword : %s\n", 
            ProgramWiFi.__SSID_STA__.c_str(), ProgramWiFi.__PASSWORD_STA__.c_str()
        );
        
        Blynk.begin(
            BLYNK_AUTH_TOKEN,
            ProgramWiFi.__SSID_STA__.c_str(),
            ProgramWiFi.__PASSWORD_STA__.c_str()
        );
        Timer.setInterval(100L, sendCapacity);
    }
}

/**
 * @brief Runs the Blynk and timer tasks.
 * @details Continuously runs the Blynk and timer tasks if the WiFi mode is STA.
 */
void Blynk_run() {
    if (WiFi.getMode() == WIFI_STA) {
        Blynk.run();
        Timer.run();
    }
}
