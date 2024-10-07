/*! @file WebServer.cpp
 *  @version 1.0.0
*/

#include <EEPROM.h>
#include "WebServer.h"
#include "main.h"
#include "variable.h"

void WebServer::RecoveryPage(AsyncWebServerRequest *req) {
  this->file = openfile("/WEB/html/recovery.html", LFS_READ);
  if (!this->file) {
    req->send(404, "text/plain", "File not found");
    return;
  }

  // read file content
  size_t fileSize = file.size();
  char* htmlBuffer = new char[fileSize + 1];
  file.readBytes(htmlBuffer, fileSize);
  htmlBuffer[fileSize] = '\0';
  file.close();

  // get ip address
  IPAddress clientIP = req->client()->localIP();
  this->LOCALIP = clientIP.toString();

  const char* placeholders[] = {
    "%LOCALIP%",
    "%VERSIONPROJECT%", "%HWVERSION%", "%SWVERSION%",
    "%BUILDDATE%", "%FIRMWAREREGION%", 
    "%LOCALIP%", "%SSIDAP%", "%PASSWORDAP%", "%SSIDSTA%", "%PASSWORDSTA%",
    "%LOCALIP%", "%LOCALIP%"
  };

  const char* tags_html[] = {
    this->LOCALIP.c_str(),
    __VERSION_PROJECT__.c_str(), __HW_VERSION__.c_str(), __SW_VERSION__.c_str(), __BUILD_DATE__.c_str(), __REGION__.c_str(),
    this->LOCALIP.c_str(), programWiFi.__SSID_AP__.c_str(), programWiFi.__PASSWORD_AP__.c_str(), programWiFi.__SSID_STA__.c_str(), programWiFi.__PASSWORD_STA__.c_str(),
    this->LOCALIP.c_str(), this->LOCALIP.c_str()
  };

  String page = htmlBuffer;
  for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++)
  {
    page.replace(placeholders[i], tags_html[i]);
  }

  req->send_P(200, "text/html", page.c_str());
  delete[] htmlBuffer;
}


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
  message += "URL: ";
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
  // TODO (Not yet implemented)
  // this platformIO Version
}

void WebServer::Serverinit() {
  if (MDNS.begin("esp8266")) {
    Serial.println(F("MDNS responder started"));
  }
  AsyncElegantOTA.begin(&serverAsync);

  using namespace std;
  // run recovery
  serverAsync.on("/", HTTP_GET, bind(&WebServer::RecoveryPage, this, placeholders::_1));

  // EnableBlynk
  serverAsync.on("/enable-blynk", HTTP_GET, bind(&WebServer::EnableBlynk, this, placeholders::_1));

  // Reboot nodemcu
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