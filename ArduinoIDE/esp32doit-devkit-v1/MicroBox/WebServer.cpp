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

void WebServerClass::ServerInit() {
    // Initialize mDNS with the hostname "esp32-delay"
    if (!MDNS.begin("esp32-delay")) {
        Serial.println(F("Error starting mDNS"));
        return;
    }
    
    // Initialize ElegantOTA for over-the-air updates
    // ElegantOTA.begin(&this->serverAsync);
	AsyncElegantOTA.begin(&this->serverAsync);

    // Serve CSS and JavaScript files
    this->Run_css_js_WebServer();

    // Setup HTTP GET endpoint for recovery page
    this->serverAsync.on("/", HTTP_GET,
        std::bind(
            &WebServerClass::RecoveryPage, this,
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
}

void WebServerClass::UpdateOTAloop() {
    // Handle OTA updates in the loop
    // ElegantOTA.loop(); // for PlatformIO
}

void WebServerClass::Run_css_js_WebServer() {
    // Define lists of CSS and JavaScript files to serve
    const std::vector<String> list_css_files = {
        "recovery.css"
    };

    const std::vector<String> list_js_file = {
        "clock.js", "reboot.js", "switchBlynk.js",
        "sweetalert.min.js"
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
