/**
 *  @file Routes.cpp
 *  @version 1.2.0
 *  @author basyair7
 *  @date 2024
 */

#include "MicroBox/WebServer.h"

void WebServerClass::Routes() {
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
}
