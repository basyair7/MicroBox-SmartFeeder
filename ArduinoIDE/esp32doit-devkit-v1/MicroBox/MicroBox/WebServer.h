/**
 *  @file WebServer.h
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

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
// #include <ElegantOTA.h>
#include <AsyncElegantOTA.h>
#include <vector>
#include <string>
#include <LittleFS.h>
#include <FS.h>
#include "info.h"
#include "MyEEPROM.hpp"

// Define MIME types for HTTP responses
#define APPJSON   "application/json"
#define TEXTHTML  "text/html"
#define TEXTPLAIN "text/plain"

extern "C" {
    // Macros for file system operations
    #define LFS          LittleFS
    #define lfsAvailable LittleFS.available
    #define lfsIsExists  LittleFS.exists
    #define openfile     LittleFS.open
    #define opendir      LittleFS.openDir
    #define removeitem   LittleFS.remove
    #define LFS_READ     "r"
    #define LFS_WRITE    "w"
}

class WebServerClass : protected Info_t {
    public:
        /**
         * Constructor to initialize the web server with a specified port.
         * @param port Port number for the web server (default: 80).
         */
        WebServerClass(int port = 80) : serverAsync(port) { this->__PORT__ = port; }

        /**
         * Initialize the web server, mDNS, and ElegantOTA.
         */
        void ServerInit(void);

        /**
         * Update loop for ElegantOTA.
         */
        void UpdateOTAloop(void);

    private:
        // Private methods for handling different web server functionalities

        /**
         * Serve the recovery page.
         * @param req Pointer to the web server request.
         */
        void RecoveryPage(AsyncWebServerRequest *req);

        /**
         * Serve CSS and JavaScript files.
         */
        void Run_css_js_WebServer(void);

        /**
         * Enable Blynk functionality.
         * @param req Pointer to the web server request.
         */
        void EnableBlynk(AsyncWebServerRequest *req);

        /**
         * Reboot the system via web request.
         * @param req Pointer to the web server request.
         */
        void RebootSys(AsyncWebServerRequest *req);

        /**
         * Handle 404 Not Found errors.
         * @param req Pointer to the web server request.
         */
        void __handleNotFound__(AsyncWebServerRequest *req);

        /**
         * Read the content of a file into a string buffer.
         * @param fileName Name of the file to read.
         * @return Content of the file as a string.
         */
        String file_buffer(String fileName);

        // Private member variables
        const String DIRCSS  = "/WEB/css/";  ///< Directory for CSS files
        const String DIRHTML = "/WEB/html/"; ///< Directory for HTML files
        const String DIRJS   = "/WEB/js/";   ///< Directory for JavaScript files

        File file;                ///< File object for file operations
        String __LOCALIP__;       ///< Local IP address of the server
        uint16_t __PORT__;        ///< Port number for the server
        AsyncWebServer serverAsync; ///< Async web server instance
        MyEEPROM myeeprom;        ///< EEPROM handler object
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WebServer)
/**
 * Global instance of WebServerClass.
 */
extern WebServerClass WebServer;
#endif
