#include "WebServer.h"

WebServer::WebServer(char *ssid, char *passphrase) :
  m_ssid(ssid),
  m_passphrase(passphrase),
  m_server(80)
{
}

/********************************************/

WebServer::~WebServer()
{
}

/********************************************/

void WebServer::handleClients(tOneWireTempDevice devices[], int numDevices)
{
  WiFiClient client = m_server.available();
  
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Access-Control-Allow-Origin: *");
          // client.println("Connection: close");
          client.println();
          
          if (numDevices == 0) {
            client.print("{}");
          }
          else
          {
            // format the output as a json string
            // sample output:
            // [{"id":":28:88:30:E1:3:0:0:F3","celsius":"21.37","fahrenheit": "70.47"},{"id":":28:8E:29:E1:3:0:0:CE","celsius":"21.25","fahrenheit": "70.25"},{"id":":28:59:5B:E1:3:0:0:61","celsius":"21.31","fahrenheit": "70.36"}]
            client.print("[");
            
            for (int x = 0; x < numDevices; x++) {
              
              client.print("{\"id\":\"");
              
              client.print(devices[x].rom_code[0], HEX);              
              for(int i = 1; i < 8; i++) {
                client.print(':');
                client.print(devices[x].rom_code[i], HEX);
              }
              
              client.print("\",\"celsius\":\"");
              client.print(devices[x].celsius);
              
              client.print("\",\"fahrenheit\": \"");
              client.print(devices[x].fahrenheit);
                
              client.print("\"}");
              
              // don't print the comma after the last sensor
              if (x + 1 != numDevices)
                client.print(",");
            }
            
            client.print("]");
            break;
          }
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
  }  
}

/********************************************/

bool WebServer::initWiFi()
{
  int status = WL_IDLE_STATUS;

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  while ( status != WL_CONNECTED) {
    
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(m_ssid);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    if ((status = WiFi.begin(m_ssid, m_passphrase)) == WL_CONNECTED)
      break;
    
    // wait 10 seconds for connection:
    delay(10000);
  } 
  
  Serial.println("begin complete");
  
  m_server.begin();
  
  printWifiStatus();
  
  return true;
}

/********************************************/

void WebServer::printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

