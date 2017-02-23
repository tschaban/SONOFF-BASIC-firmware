/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_EEPROM_h
#define _sonoff_EEPROM_h

#include <EEPROM.h>
#include "sonoff-configuration.h"

#define EEPROM_SIZE 512

class SonoffEEPROM
{
  private:
    int EEPROM_size = 512;
    void write(int address, int size, String in);
    String read(int address, int size);
    void clear(int start, int size);
    void setDefaults();

  public:
    SonoffEEPROM();

    SONOFFCONFIG getConfiguration();
   
    uint8_t getRelayState();
    uint8_t getRelayStateAfterPowerRestored();
    uint8_t getRelayStateAfterConnectionRestored();
    boolean isDS18B20Present();
    float   DS18B20Correction();
    unsigned int DS18B20ReadInterval();
    void saveDeviceName(String in);
    void saveVersion(String in);
    void saveLanguage(String in);
    void saveWiFiSSID(String in);
    void saveWiFiPassword(String in);
    void saveMQTTHost(String in);
    void saveMQTTPort(unsigned int in);
    void saveMQTTUser(String in);
    void saveMQTTPassword(String in);
    void saveMQTTTopic(String in);
    void saveMode(int in); /* Look at MODE_... defined */
    void saveTemperatureCorrection(float in);
    void saveTemperatureInterval(unsigned int in);
    void saveTemperatureSensorPresent(unsigned int in);
    void saveRelayState(unsigned int in);
    void saveRelayStateAfterPowerRestored(unsigned int in);
    void saveRelayStateAfterConnectionRestored(unsigned int in);
    void erase();
};
#endif


