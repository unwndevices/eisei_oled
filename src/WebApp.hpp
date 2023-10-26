#ifndef WEBAPP_HPP
#define WEBAPP_HPP

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include "SharedContext.hpp"

const char *indexHTML = "/index.html";

class WebApp
{
public:
    WebApp(SharedContext &context) : server(80), context(context) {}

    void Init()
    {
        server.on("/", HTTP_GET, [this]()
                  {
        if (!this->handleFileRead(indexHTML))
        {
            server.send(404, "text/plain", "FileNotFound");
        } });

        server.on("/index.html", HTTP_GET, [this]()
                  {
        if (!this->handleFileRead(indexHTML))
        {
            server.send(404, "text/plain", "FileNotFound");
        } });

        server.serveStatic("/", LittleFS, "/");
        server.begin();
    }

    void Update()
    {
        server.handleClient();
    }

private:
    WebServer server;
    SharedContext &context;

    bool exists(String path);
    String getContentType(String filename);

    bool handleFileRead(String path);
};

bool WebApp::handleFileRead(String path)
{
    if (exists(path))
    {
        File file = LittleFS.open(path, "r");
        server.streamFile(file, getContentType(path));
        file.close();
        return true;
    }
    return false;
}

bool WebApp::exists(String path)
{
    return LittleFS.exists(path);
}

String WebApp::getContentType(String filename)
{
    if (server.hasArg("download"))
    {
        return "application/octet-stream";
    }
    else if (filename.endsWith(".json"))
    {
        return "application/json";
    }
    else if (filename.endsWith(".htm"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".html"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".css"))
    {
        return "text/css";
    }
    else if (filename.endsWith(".js"))
    {
        return "application/javascript";
    }
    else if (filename.endsWith(".png"))
    {
        return "image/png";
    }
    else if (filename.endsWith(".gif"))
    {
        return "image/gif";
    }
    else if (filename.endsWith(".jpg"))
    {
        return "image/jpeg";
    }
    else if (filename.endsWith(".ico"))
    {
        return "image/x-icon";
    }
    else if (filename.endsWith(".xml"))
    {
        return "text/xml";
    }
    else if (filename.endsWith(".pdf"))
    {
        return "application/x-pdf";
    }
    else if (filename.endsWith(".zip"))
    {
        return "application/x-zip";
    }
    else if (filename.endsWith(".html.gz"))
    {
        return "text/html";
    }
    return "text/plain";
}

#endif // WEBAPP_HPP
