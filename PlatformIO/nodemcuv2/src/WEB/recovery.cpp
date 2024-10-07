#include "WebServer.h"
#include "main.h"

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
        __VERSION_PROJECT__.c_str(), __HW_VERSION__.c_str(), __SW_VERSION__.c_str(),
        __BUILD_DATE__.c_str(), __REGION__.c_str(),
        this->LOCALIP.c_str(), programWiFi.__SSID_AP__.c_str(), programWiFi.__PASSWORD_AP__.c_str(),
        programWiFi.__SSID_STA__.c_str(), programWiFi.__PASSWORD_STA__.c_str(),
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