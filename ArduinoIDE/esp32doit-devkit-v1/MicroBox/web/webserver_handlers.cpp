/**
 *  @file webserver_handlers.cpp
 *  @version 1.2.0
 *  @author basyair7
 *  @date 2024
 */

#include "../MicroBox/WebServer.h"
#include "../MicroBox/externprog.h"

String WebServerClass::file_buffer(String fileName) {
    this->file = openfile(fileName, LFS_READ);
    if (!this->file) {
        return "";
    }

    // read file content
    size_t fileSize = this->file.size();
    std::unique_ptr<char []> fileBuffer(new char[fileSize + 1]);
    this->file.readBytes(fileBuffer.get(), fileSize);
    fileBuffer[fileSize] = '\0';
    this->file.close();

    return String(fileBuffer.get());
}

void WebServerClass::RebootSys(AsyncWebServerRequest *req) {
    StaticJsonDocument<100> doc;
    std::string jsonResponse = "";
    uint16_t codeRes = 200;

    doc["status"] = codeRes;
    doc["msg"] = "Server has been restart";

    serializeJson(doc, jsonResponse);
    req->send_P(codeRes, APPJSON, jsonResponse.c_str());

    LastTimeReboot = millis();
    RebootState = true;
}

void WebServerClass::EnableBlynk(AsyncWebServerRequest *req) {
    StaticJsonDocument<100> doc;

    std::string jsonResponse = "";
    uint16_t codeRes = 200;

    doc["status"] = codeRes;
    doc["msg"] = "Enable Blynk, Server has been restart";

    serializeJson(doc, jsonResponse);
    req->send_P(codeRes, APPJSON, jsonResponse.c_str());

    this->myeeprom.save_wifi_state(true);
    delay(50);

    LastTimeReboot = millis();
    RebootState = true;
}

// page 404
void WebServerClass::__handleNotFound__(AsyncWebServerRequest *req) {
    StaticJsonDocument<200> doc;

    std::string jsonRes = "";
    uint16_t codeRes = 404;

    doc["status"] = codeRes;
    doc["msg"] = "File not found";
    doc["uri"] = req->url();
    doc["method"] = req->method() == HTTP_GET ? "GET" : "POST";
    doc["args_count"] = req->args();

    JsonObject args = doc.createNestedObject("args");
    for (uint8_t i = 0; i < req->args(); i++) {
        args[req->argName(i)] = req->arg(i);
    }

    serializeJson(doc, jsonRes);
    req->send_P(codeRes, APPJSON, jsonRes.c_str());
}
