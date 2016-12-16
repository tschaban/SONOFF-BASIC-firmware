/* EPPROM MAP
 */
#ifndef SonoffEEPROM_h
#define SonoffEEPROM_h

#include <EEPROM.h>

class SonoffEEPROM
{
  public:
    SonoffEEPROM();
    String getWiFiSSID();
    String getWiFiPassword();
    String getMQTTHost();
    String getMQTTPort();
    String getMQTTUser();
    String getMQTTPassword();
    void saveWiFiSSID(String in);
    void saveWiFiPassword(String in);
    void saveMQTTHost(String in);
    void saveMQTTPort(String in);
    void saveMQTTUser(String in);
    void saveMQTTPassword(String in);
  private:
    void write(int address, int size, String in);
    String read(int address, int size);
    void clear(int start, int size);
};
#endif


