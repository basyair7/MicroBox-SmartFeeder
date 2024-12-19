/**
 *  @file recovery.cpp
 *  @version 1.2.0
 *  @author basyair7
 *  @date 2024
 */

#include "../MicroBox/WebServer.h"
#include "../MicroBox/ProgramWiFi.h"

void WebServerClass::RecoveryPage(AsyncWebServerRequest *req) {
    String page = this->file_buffer(this->DIRHTML + "recovery.html");
    if (page == "") {
        this->__handleNotFound__(req);
        return;
    }

    // get IP Address
    this->__LOCALIP__ = req->client()->localIP().toString().c_str();

    const std::string placeholders[] = {
        "%LOCALIP%",
        "%VERSIONPROJECT%", "%HWVERSION%", "%SWVERSION%",
        "%BUILDDATE%", "%FIRMWAREREGION%",
        "%LOCALIP%", "%SSIDAP%", "%PASSWORDAP%", "%SSIDSTA%",
        "%PASSWORDSTA%",
        "%LOCALIP%", "%LOCALIP%"
    };

    const std::string tags_html[] = {
        this->__LOCALIP__.c_str(),
        this->projectVersion.c_str(),
        this->hardwareVersion.c_str(),
        this->softwareVersion.c_str(),
        this->buildDate.c_str(),
        this->regionName.c_str(),
        
        this->__LOCALIP__.c_str(),
        ProgramWiFi.__SSID_AP__.c_str(),
        ProgramWiFi.__PASSWORD_AP__.c_str(),
        ProgramWiFi.__SSID_STA__.c_str(),
        ProgramWiFi.__PASSWORD_STA__.c_str(),

        this->__LOCALIP__.c_str(), this->__LOCALIP__.c_str()
    };

    // Replace page
    for (size_t i = 0; i < std::size_t(tags_html); ++i) {
        page.replace(placeholders[i].c_str(), tags_html[i].c_str());
    }

    req->send_P(200, TEXTHTML, page.c_str());
}
