/**
 *  @file WebServer.cpp
 *  @version 1.0.1
 *  @date 2024
 *  @copyright
 *  Copyright (C) 2024, basyair7
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "MicroBox/WebServer.h"
#include "MicroBox/ProgramWiFi.h"

void WebServerClass::ServerInit() {
    // Initialize LittleFS
    while (!LFS.begin()) {
        Serial.println(F("Failed..."));
        Serial.println(F("Error initializing LittleFS, please try again..."));
        Serial.println(F("Done : Error 0x1"));
        delay(150);
    }

    // Initialize mDNS with the hostname "esp32-delay"
    if (!MDNS.begin("esp32-delay")) {
        Serial.println(F("Error starting mDNS"));
        return;
    }
    
    // Initialize ElegantOTA for over-the-air updates
    ElegantOTA.begin(&this->serverAsync);

    // setup WebSocket
    this->ws.onEvent(std::bind(
        &WebServerClass::onEvent, this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3,
        std::placeholders::_4,
        std::placeholders::_5,
        std::placeholders::_6
    ));

    // add Handler WebSocket on serverAsync
    this->serverAsync.addHandler(&this->ws);

    // Serve CSS and JavaScript files
    this->Run_css_js_WebServer();

    // Setup HTTP GET endpoint for recovery page
    this->serverAsync.on("/recovery", HTTP_GET,
        std::bind(
            &WebServerClass::RecoveryPage, this,
            std::placeholders::_1
        )
    );

    // Setup HTTP GET endpoint for data server
    this->serverAsync.on("/data-server", HTTP_GET, 
        std::bind(
            &WebServerClass::DataWebServer, this,
            std::placeholders::_1
        )
    );

    // Setup HTTP GET endpoint to config rtc module
    this->serverAsync.on("/config-rtc", HTTP_GET,
        std::bind(
            &WebServerClass::RTC_Config_Main, this, 
            std::placeholders::_1
        )
    );

    // Setup HTTP GET endpoint to save config rtc module
    this->serverAsync.on("/save-config-rtc", HTTP_POST, 
        std::bind(
            &WebServerClass::Save_RTC_Config, this,
            std::placeholders::_1
        )
    );

    // Setup HTTP GET endpoint to enable Blynk
    this->serverAsync.on("/enable-blynk", HTTP_GET,
        std::bind(
            &WebServerClass::EnableBlynk, this,
            std::placeholders::_1
        )
    );

    // Setup HTTP GET endpoint to reboot the system
    this->serverAsync.on("/rst-webserver", HTTP_GET, 
        std::bind(
            &WebServerClass::RebootSys, this,
            std::placeholders::_1
        )
    );

    // initializes AsyncWebServer
    this->serverAsync.begin();
    Serial.println(F("HTTP Started..."));
    this->__LOCALIP__ = ProgramWiFi.__LOCALIPServer__;
    Serial.printf("http://%s:%d/index\t-> index page\n", this->__LOCALIP__.c_str(), this->__PORT__);
    Serial.printf("http://%s:%d/recovery\t-> recovery page\n", this->__LOCALIP__.c_str(), this->__PORT__);
}

void WebServerClass::UpdateOTAloop() {
    // Handle OTA updates in the loop
    ElegantOTA.loop();
}

void WebServerClass::Run_css_js_WebServer() {
    // Define lists of CSS and JavaScript files to serve
    const std::vector<String> list_css_files = {
        "recovery.css", "config_rtc.css"
    };

    const std::vector<String> list_js_file = {
        "clock.js", "reboot.js", "switchBlynk.js",
        "sweetalert.min.js", "date_time_rtc.js"
    };

    // Serve each CSS file
    for (const auto fileName : list_css_files) {
        this->serverAsync.serveStatic(
            ("/css/" + fileName).c_str(), LFS,
            (this->DIRCSS + fileName).c_str()
        );
    }

    // Serve each JavaScript file
    for (const auto fileName : list_js_file) {
        this->serverAsync.serveStatic(
            ("/js/" + fileName).c_str(), LFS,
            (this->DIRJS + fileName).c_str()
        );
    }
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WebServer)
/**
 * Create a global instance of WebServerClass.
 */
WebServerClass WebServer;
#endif
