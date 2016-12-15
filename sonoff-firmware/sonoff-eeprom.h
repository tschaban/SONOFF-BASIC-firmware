#ifndef SonoffEEPROM_h
#define SonoffEEPROM_h

#include <EEPROM.h>

class SonoffEEPROM
{
  public:
    SonoffEEPROM();
    void read();
    void saveWiFiSSID(String in);
    void saveWiFiPassword(String in);    
  private:
    void write(int address, int size, String in);
    void clear(int start, int size);
};

#endif
