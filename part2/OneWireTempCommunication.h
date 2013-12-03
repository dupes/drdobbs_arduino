
#ifndef ONE_WIRE_TEMP_H
#define ONE_WIRE_TEMP_H

#include <OneWire.h>

#define MAX_DEVICES 10

struct tOneWireTempDevice {
  byte rom_code[8];
  float celsius;
  float fahrenheit;
};

enum eState {
  SEARCH,
  SEARCH_DELAY,
  SKIP_AND_CONVERT,
  DELAY_FOR_CONVERT,
  READ_NEXT
};

class OneWireTempCommunication {
  private:
    int m_pin;
    
    OneWire m_oneWire;
  
    eState m_state;
    
    // state machine functions
    
    eState searchForDevices();
    eState searchDelay();
    eState skipAndConvert();
    eState delayForConvert();
    eState readNext();
    
  public:
    OneWireTempCommunication(int pin);
    ~OneWireTempCommunication();
    
    void resetState();
    
    void pollDevices();

    tOneWireTempDevice devices[MAX_DEVICES];
    int numDevices;
    

};

#endif
