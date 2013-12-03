#include "OneWireTempCommunication.h"

OneWireTempCommunication::OneWireTempCommunication(int pin) : 
  m_pin(pin),
  m_oneWire(m_pin),
  m_state(SEARCH)
{
    numDevices = 0;
}

/********************************************/

OneWireTempCommunication::~OneWireTempCommunication()
{
}

/********************************************/

eState OneWireTempCommunication::skipAndConvert()
{
  m_oneWire.reset();
  
  // send the skip command; all devices on the bus will respond
  // to the next command
  m_oneWire.write(0xcc);
  
  // tell all the devices to start converting; 1 means use paraiste power
  m_oneWire.write(0x44, 1);
  
  return DELAY_FOR_CONVERT;
}

/********************************************/

eState OneWireTempCommunication::delayForConvert()
{
  static bool delayStart = false;
  static unsigned long startTime = 0;
  
  if (!delayStart)
  {
    delayStart = true;
    startTime = millis();
  }
  
  if (millis() - startTime >= 1000)
  {
    delayStart = false;
    return READ_NEXT;
  }
    
  return DELAY_FOR_CONVERT;
}

/********************************************/

eState OneWireTempCommunication::readNext()
{
  byte data[9];
  static int currentDevice = 0;
  
  if (currentDevice == numDevices)
  {
    Serial.println();
    currentDevice = 0;
    return SKIP_AND_CONVERT;
  }
 
  byte present = m_oneWire.reset();
  m_oneWire.select(devices[currentDevice].rom_code);    
  m_oneWire.write(0xBE);         // Read Scratchpad

  for (int i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = m_oneWire.read();
  }
  
  Serial.println();
  
  byte crc = OneWire::crc8(data, 8);
  
  // only record the reading if the crc matched
  if (crc == data[8])
  {
    unsigned int raw = (data[1] << 8) | data[0];
    
    devices[currentDevice].celsius = (float)raw / 16.0;
    devices[currentDevice].fahrenheit = devices[currentDevice].celsius * 1.8 + 32.0;
    
    for(int i = 0; i < 8; i++) {
      Serial.write(' ');
      Serial.print(devices[currentDevice].rom_code[i], HEX);
    }
    
    Serial.print(":  Temperature = ");
    Serial.print(devices[currentDevice].celsius);
    Serial.print(" Celsius, ");
    Serial.print(devices[currentDevice].fahrenheit);
    Serial.print(" Fahrenheit");
  
  }
  else
  {
    Serial.print(" CRC error=");
    Serial.print(crc, HEX);
    Serial.print(" ");        
    Serial.println(data[9]);
  }
  
  currentDevice++;
  return READ_NEXT;
}

/********************************************/

eState OneWireTempCommunication::searchDelay()
{
  static bool delayStart = false;
  static unsigned long startTime = 0;
  
  if (!delayStart)
  {
    delayStart = true;
    startTime = millis();
  }
  
  if (millis() - startTime >= 1000)
  {
    delayStart = false;
    return SEARCH;
  }
  
  return SEARCH_DELAY;
}
    
/********************************************/
    
eState OneWireTempCommunication::searchForDevices()
{  
  if (m_oneWire.reset() != 1)
  {
    Serial.println("No devices asserted line");
    return SEARCH_DELAY;
  }
    
  //m_oneWire.reset_search();    
  if ( !m_oneWire.search(devices[numDevices].rom_code)) 
  {
    Serial.println("No more addresses.");
    m_oneWire.reset_search();
    
    return SKIP_AND_CONVERT;
  }
    
  for(int i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(devices[numDevices].rom_code[i], HEX);
  }
  Serial.println();
    
  byte crc = OneWire::crc8(devices[numDevices].rom_code, 7);
        
  if (crc != devices[numDevices].rom_code[7]) {
      Serial.print("invalid CRC on rom code: ");
      Serial.print(crc, HEX);
      Serial.print(" ");
      Serial.println(devices[numDevices].rom_code[7]);
  }
  else if (numDevices++ == MAX_DEVICES)
  {      
    Serial.println("max devices found\n");
    return SKIP_AND_CONVERT;
  }
  
  return SEARCH;
}

/********************************************/

void OneWireTempCommunication::resetState()
{
  numDevices = 0;
  m_state = SEARCH;
}
    
/********************************************/

void OneWireTempCommunication::pollDevices()
{
  switch (m_state)
  {
  case SEARCH:
    m_state = searchForDevices();
    break;
  
  case SEARCH_DELAY:
    m_state = searchDelay();
    break;
    
  case SKIP_AND_CONVERT:
    m_state = skipAndConvert();
    break;
    
  case DELAY_FOR_CONVERT:
    m_state = delayForConvert();
    break;
  
  case READ_NEXT:
    m_state = readNext();
    break;
  }
}
