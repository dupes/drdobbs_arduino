#include <OneWire.h>
#include <WiFi.h>

// This scketch was adapted from OneWire DS18x20_Temperature sample program distirbuted with the
// Ardunio OneWire library, and the WiFiWebServer example distributed with the Arduino 1.0.4 download

/********************************************/

#include "OneWireTempCommunication.h"
#include "WebServer.h"

/********************************************/

int deviceCount = 0;
byte data[12];
byte addr[8];

// modify these lines to match your system
OneWireTempCommunication comm(2);
WebServer webServer("ssid", "passphrase");

/********************************************/

void setup() {
  Serial.begin(9600);
  
  webServer.initWiFi();
}

/********************************************/

void loop() {
    
  comm.pollDevices();
  
  webServer.handleClients(comm.devices, comm.numDevices);
}

