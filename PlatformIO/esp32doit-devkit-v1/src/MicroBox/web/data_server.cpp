/**
 *  @file data_server.cc
 *  @version 1.0.1
 *  @author basyair7
 *  @date 2024
*/

#include <ArduinoJson.h>
#include "MicroBox/WebServer.h"
#include "MicroBox/externprog.h"

void WebServerClass::__getDataServer__(DynamicJsonDocument &doc) {
    StaticJsonDocument<500> dataRelay;

    JsonObject data = doc.createNestedObject("data_server");
    // uint8_t hour, minute;
    // rtcprog.time(&hour, &minute);

    data["date"]        = rtcprog.datestr();
    // data["time"]        = String(hour) + ":" + String(minute);
    data["time"]        = rtcprog.timestr();
}

// webserver program
void WebServerClass::DataWebServer(AsyncWebServerRequest *req) {
    DynamicJsonDocument doc(500);
    String jsonBuffer = "";
    int codeRes = 200;
    
    doc["status"] = codeRes;
    doc["heap_memory"]["total_heap"] = String(totalHeapMemory, 2);
    doc["heap_memory"]["free_heap"] = String(freeHeapMemory, 2);
    this->__getDataServer__(doc);
    
    serializeJson(doc, jsonBuffer);
    doc.clear();
    req->send_P(codeRes, APPJSON, jsonBuffer.c_str());
}


// websocket program
void WebServerClass::handleDataServerWS(AsyncWebSocketClient *client) {
    DynamicJsonDocument response(500);
    response["event"] = "data_server";
    response["heap_memory"]["total_heap"] = String(totalHeapMemory, 2);
    response["heap_memory"]["free_heap"] = String(freeHeapMemory, 2);
    this->__getDataServer__(response);

    String jsonResponse = "";
    serializeJson(response, jsonResponse);
    response.clear();
    client->text(jsonResponse);
}
