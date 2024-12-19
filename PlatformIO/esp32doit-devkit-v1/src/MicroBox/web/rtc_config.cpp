/**
 * @file rtc_config.cc
 * @version 1.0.1
 * @author basyair7
 * @date 2024 
*/

#include "MicroBox/WebServer.h"
#include "MicroBox/externprog.h"

void WebServerClass::RTC_Config_Main(AsyncWebServerRequest *req) {
    // read file html
    String page = this->file_buffer(this->DIRHTML + "config_rtc.html");
    if (page == "") {
        this->__handleNotFound__(req);
        return;
    }

    // get ip address
    this->__LOCALIP__ = req->client()->localIP().toString();

    const std::string placeholders[] = {
        "%VERSIONPROJECT%",
        "%LOCALIP%"
    };

    const std::string tags_html[] = {
        this->projectVersion.c_str(),
        this->__LOCALIP__.c_str()
    };

    // replace page
    for (size_t i = 0; i < std::size(tags_html); ++i) {
        page.replace(placeholders[i].c_str(), tags_html[i].c_str());
    }

    req->send_P(200, TEXTHTML, page.c_str());
}

void WebServerClass::Save_RTC_Config(AsyncWebServerRequest *req) {
    // read file html
    String page = this->file_buffer(this->DIRHTML + "save_config_rtc.html");
    if (page == "") {
        this->__handleNotFound__(req);
        return;
    }

    int __MONTH__, __DAY__, __YEAR__;
    int __HOUR__, __MINUTE__, __SECOND__;

    if (req->hasArg("date") && req->hasArg("time") && req->hasArg("seconds")) {
        String date = req->arg("date");   // Format: YYYY-MM-DD
        String time = req->arg("time");   // Format: HH:MM
        
        // Check 12-hour format
        rtcprog.twelve_hour_format_flag = req->hasArg("twelve_hour_format");

        // Parse date
        __YEAR__  = date.substring(0, 4).toInt();
        __MONTH__ = date.substring(5, 7).toInt();
        __DAY__   = date.substring(8, 10).toInt();

        // Parse time
        __HOUR__   = time.substring(0, 2).toInt();
        __MINUTE__ = time.substring(3, 5).toInt();
        __SECOND__ = req->arg("seconds").toInt();

        rtcprog.manualAdjust(
            __MONTH__, __DAY__, __YEAR__,
            __HOUR__, __MINUTE__, __SECOND__
        );
    }

    // get ip address
    this->__LOCALIP__ = req->client()->localIP().toString();

    const std::string placeholders[] = {
        "%VERSIONPROJECT%",
        "%LOCALIP%"
    };

    const std::string tags_html[] = {
        this->projectVersion.c_str(),
        this->__LOCALIP__.c_str()
    };

    // replace page
    for (size_t i = 0; i < std::size(tags_html); ++i) {
        page.replace(placeholders[i].c_str(), tags_html[i].c_str());
    }

    req->send_P(200, TEXTHTML, page.c_str());
}
