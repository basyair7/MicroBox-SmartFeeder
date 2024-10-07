/*! @file WebServer.hpp
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <ElegantOTA.h>
#include <vector>
#include "TypeLittleFS"
#include "info.h"


class WebServer : protected Info {
public:
	WebServer(int port) : serverAsync(port) { /* TODO(not yet implemented) */ }
	void Serverinit();
	void updateOTAloop();

private:
	void Reboot(AsyncWebServerRequest *req);
	void EnableBlynk(AsyncWebServerRequest *req);
	void handleNotFound(AsyncWebServerRequest *req);
	void RecoveryPage(AsyncWebServerRequest *req);
	
private:
	File file;
	String LOCALIP;
	AsyncWebServer serverAsync;
};