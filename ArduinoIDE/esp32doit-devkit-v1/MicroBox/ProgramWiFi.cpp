/**
 *  @file ProgramWiFi.cpp
 *  @version 1.2.0
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

#include "MicroBox/ProgramWiFi.h"
#include "MicroBox/SysHandlers.h"

// run program if WiFi connecting
void ProgramWiFiClass::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println(F("\nConnection to AP Successfully"));
}

// run program if wifi disconnected
void ProgramWiFiClass::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    static unsigned long LastMillis = 0;
    if ((unsigned long) (millis() - LastMillis) >= 15000L && WiFi.status() != WL_CONNECTED)
    {
        LastMillis = millis();
        Serial.println(F("Disconnected from WiFi Access Point"));
        Serial.print(F("WiFi lost connection, Reason "));
        Serial.println(info.wifi_sta_disconnected.reason);
        WiFi.begin(this->__SSID_STA__.c_str(), this->__PASSWORD_STA__.c_str());
    }
}

// got ip address if wifi connected
void ProgramWiFiClass::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    this->__LOCALIPServer__ = WiFi.localIP().toString().c_str();
    Serial.println(F("WiFi Connected"));
    Serial.print(F("IP Address: "));
    Serial.println(this->__LOCALIPServer__);
}

// Run program Mode STA
void ProgramWiFiClass::__wifi_mode_sta__() {
    // Setup WiFi
    // WiFi.mode(WIFI_AP_STA);
    WiFi.mode(WIFI_STA);
    this->__bootBtn__.begin();

    // Disable sleep mode
    WiFi.setSleep(false);

    // register event handlers
    WiFi.onEvent(
        std::bind(
            &ProgramWiFiClass::WiFiStationConnected, this, 
            std::placeholders::_1, std::placeholders::_2
        ), ARDUINO_EVENT_WIFI_STA_CONNECTED
    );
    WiFi.onEvent(
        std::bind(
            &ProgramWiFiClass::WiFiGotIP, this,
            std::placeholders::_1,
            std::placeholders::_2
        ), ARDUINO_EVENT_WIFI_STA_GOT_IP
    );
    WiFi.onEvent(
        std::bind(
            &ProgramWiFiClass::WiFiStationDisconnected, this,
            std::placeholders::_1,
            std::placeholders::_2
        ), ARDUINO_EVENT_WIFI_STA_DISCONNECTED
    );

    // Attempt to connect to WiFi
    WiFi.begin(this->__SSID_STA__.c_str(), this->__PASSWORD_STA__.c_str());
    Serial.println(F("Connecting to WiFi..."));
    while (WiFi.status() != WL_CONNECTED) {
        this->__bootBtn__.ChageWiFiMode();
        AutoChangeState::run();
        delay(500);
    }

    // RSSI
    Serial.print(F("\nRSSI: "));
    Serial.println(WiFi.RSSI());
}

// run program mode AP
void ProgramWiFiClass::__wifi_mode_ap__() {
    // setup wifi mode AP
    WiFi.mode(WIFI_AP);

    // initializing Access Point
    WiFi.softAP(this->__SSID_AP__.c_str(), this->__PASSWORD_AP__.c_str());

    // get IP Address
    this->__LOCALIPServer__ = WiFi.softAPIP().toString().c_str();
    Serial.print(F("IP Address: "));
    Serial.println(this->__LOCALIPServer__);
    Serial.println();
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_ProgramWiFi)
ProgramWiFiClass ProgramWiFi;
#endif