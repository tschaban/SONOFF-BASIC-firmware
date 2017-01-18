/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_EEPROM_h
#define _sonoff_EEPROM_h

#include <EEPROM.h>

class SonoffEEPROM
{
  private: 
    int EEPROM_size = 512;
    void write(int address, int size, String in);
    String read(int address, int size);
    void clear(int start, int size);
    void setDefaults();
    char* getChar(int start, int size);
    
   
  public:
    SonoffEEPROM();    
    SONOFFCONFIG getConfiguration();

    unsigned int getRelayState();
    char* getVersion();

    char* getID();
    char* getHostName();

    char* getWiFiSSID();
    char* getWiFiPassword();

    char* getMQTTHost();
    int getMQTTPort();
    char* getMQTTUser();
    char* getMQTTPassword();
    char* getMQTTTopic();

    boolean isDS18B20Present();
    float   DS18B20Correction();
    unsigned int DS18B20ReadInterval();

    void saveVersion(String in);    
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
    void saveTemperatureSensorPresent(unsigned int in);    
    void saveRelayState(unsigned int in);

    void erase();    
};
#endif


