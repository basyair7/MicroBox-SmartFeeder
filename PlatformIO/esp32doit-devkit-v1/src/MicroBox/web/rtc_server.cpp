/**
 *  @file rtc_server.cc
 *  @version 1.0.1
 *  @author basyair7
 *  @date 2024
*/

#include <ArduinoJson.h>
#include "MicroBox/WebServer.h"
#include "MicroBox/externprog.h"
#include "MicroBox/info.h"

void WebServerClass::__getRTCServer__(StaticJsonDocument<200> &doc) {
    JsonObject _datetime = doc.createNestedObject("datetime");
    _datetime["date"] = rtcprog.datestr();
    _datetime["time"] = rtcprog.timestr();
}

void WebServerClass::RTCServer(AsyncWebServerRequest *req) {
    StaticJsonDocument<200> doc;
    String response = "";
    uint16_t statusCode = 200;

    doc["status"] = statusCode;
    this->__getRTCServer__(doc);

    serializeJson(doc, response);
    req->send_P(statusCode, APPJSON, response.c_str());
}

void WebServerClass::handleRTCServer(AsyncWebSocketClient *client) {
    StaticJsonDocument<200> response;
    response["event"] = "datetime";
    response["heap_memory"]["total_heap"] = String(totalHeapMemory, 2);
    response["heap_memory"]["free_heap"] = String(freeHeapMemory, 2);
    this->__getRTCServer__(response);

    String jsonResponse = "";
    serializeJson(response, jsonResponse);
    response.clear();
    client->text(jsonResponse.c_str());
}
