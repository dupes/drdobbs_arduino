
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WiFi.h>

#include "OneWireTempCommunication.h"

class WebServer {
  private:
    
    char *m_ssid;
    char *m_passphrase;
    
    WiFiServer m_server;
    
  public:
  
    WebServer(char *ssid, char *passphrase);
    ~WebServer();
    
    void handleClients(tOneWireTempDevice devices[], int numDevices);
    
    bool initWiFi();
    
    void printWifiStatus();
};

#endif
