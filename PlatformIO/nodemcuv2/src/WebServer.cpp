/*! @file WebServer.cpp
 *  @version 1.0.0
*/

#include <EEPROM.h>
#include "WebServer.h"
#include "variable.h"

unsigned long LastReboot = 0, LastChangeMode = 0;
void WebServer::Reboot(AsyncWebServerRequest *req) {
	const String page = "{\"status\": \"Server has been restart\"}";
	req->send_P(200, "application/json", page.c_str());
	if (millis() - LastReboot >= 5000) {
		LastReboot = millis();
		ESP.restart();
	}
}

void WebServer::EnableBlynk(AsyncWebServerRequest *req) {
	const String page = "{\"status\": \"Enable Blynk, Server has been restart\"}";
	req->send_P(200, "application/json", page.c_str());
	if (millis() - LastChangeMode >= 5000) {
		LastChangeMode = millis();
		EEPROM.put(EEPROM_BLYNK_ADDR, false);
		EEPROM.commit();
		delay(50);
		ESP.restart();
	}
}

// page 404
void WebServer::handleNotFound(AsyncWebServerRequest *req) {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += req->url();
	message += "\nMethod: ";
	message += (req->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += req->args();
	message += "\n";
	for (uint8_t i = 0; i < req->args(); i++) {
		message += " " + req->argName(i) + ": " + req->arg(i) + "\n";
	}
	req->send(404, "text/plain", message);
}

void WebServer::updateOTAloop() {
	ElegantOTA.loop();
}

void WebServer::Serverinit() {
	if (MDNS.begin("esp8266")) {
		Serial.println(F("MDNS responder started"));
	}
	ElegantOTA.begin(&serverAsync);
	
	using namespace std;
	// run recovery
	serverAsync.on("/", HTTP_GET, bind(&WebServer::RecoveryPage, this, placeholders::_1));
	
	// EnableBlynk
	serverAsync.on("/enable-blynk", HTTP_GET, bind(&WebServer::EnableBlynk, this, placeholders::_1));
	
	// reboot web serverAsync
	serverAsync.on("/rst-webserver", HTTP_GET, bind(&WebServer::Reboot, this, placeholders::_1));
	
	// get css file and javascripts file
	// css
	serverAsync.serveStatic("/css/recovery.css", LFS, "/WEB/css/recovery.css");
	// js
	serverAsync.serveStatic("/js/clock.js", LFS, "/WEB/js/clock.js");
	serverAsync.serveStatic("/js/reboot.js", LFS, "/WEB/js/reboot.js");
	serverAsync.serveStatic("/js/switchBlynk.js", LFS, "/WEB/js/switchBlynk.js");
	serverAsync.serveStatic("/js/sweetalert.min.js", LFS, "/WEB/js/sweetalert.min.js");
	
	// if page not found
	serverAsync.onNotFound(bind(&WebServer::handleNotFound, this, placeholders::_1));
	
	// run server
	serverAsync.begin();
	Serial.println(F("HTTP Server Started"));
}