/**
 *  @file ProgramWiFi.h
 *  @version 1.0.1
 *  @author basyair7
 *  @date 2024
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

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <functional>
#include "MyEEPROM.hpp"
#include "BootButton.h"
#include "variable.h"

class ProgramWiFiClass {
    public:
        String __SSID_STA__, __PASSWORD_STA__;
        String __SSID_AP__, __PASSWORD_AP__;
        String __LOCALIPServer__;

    public:
        void setup(const String ssid_sta, const String password_sta, 
                   const String ssid_ap, const String password_ap) 
        {
            this->__SSID_STA__     = ssid_sta;
            this->__SSID_AP__      = ssid_ap;
            this->__PASSWORD_STA__ = password_sta;
            this->__PASSWORD_AP__  = password_ap;
        }

        /**
         *  @brief initialize wifi function
         *  @param state Mode_AP = `true` or Mode_STA = `false`
         */
        void initWiFi(bool state) {
            Serial.printf("\nMode WiFi: %s\n", state ? "WIFI_STA" : "WIFI_AP");
            if (state) {
                this->__wifi_mode_sta__();
            }
            else {
                this->__wifi_mode_ap__();
            }
        }

    private:
        void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
        void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
        void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
        void __wifi_mode_ap__(), __wifi_mode_sta__();

        IPAddress local_ip;
        IPAddress gateway;
        IPAddress subnet;

        BootButton __bootBtn__ = BootButton(BOOTBUTTON, INPUT);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_ProgramWiFi)
extern ProgramWiFiClass ProgramWiFi;
#endif