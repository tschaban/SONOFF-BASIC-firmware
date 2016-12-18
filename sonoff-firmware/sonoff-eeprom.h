/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef SonoffEEPROM_h
#define SonoffEEPROM_h

#include <EEPROM.h>

class SonoffEEPROM
{
  public:
    SonoffEEPROM();
    void saveWiFiSSID(String in);
    void saveWiFiPassword(String in);
    void saveMQTTHost(String in);
    void saveMQTTPort(unsigned int in);
    void saveMQTTUser(String in);
    void saveMQTTPassword(String in);
    void saveMQTTTopic(String in);     
    void saveSwitchMode(int in);
    void saveTemperatureCorrection(float in); 
    void saveTemperatureInterval(unsigned int in);        
    String getWiFiSSID();
    String getWiFiPassword();
    String getMQTTHost();
    String getMQTTPort();
    String getMQTTUser();
    String getMQTTTopic();
    String getMQTTPassword();
    String getSwitchMode();
    String getVersion();
    String getTemperatureCorrection(); 
    String getTemperatureInterval(); 
    SONOFFCONFIG getConfiguration();
    void erase();
  private: 
    int EEPROM_size = 512;
    void write(int address, int size, String in);
    String read(int address, int size);
    void clear(int start, int size);
    void setDefaults();
    

    
};
#endif


