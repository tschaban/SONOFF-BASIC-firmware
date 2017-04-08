/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_EEPROM_h
#define _sonoff_EEPROM_h

#include <EEPROM.h>

#define EEPROM_SIZE 512

class SonoffEEPROM
{
  private:
    int EEPROM_size = 512;
    void write(unsigned int address, unsigned int size, String in);
    String read(unsigned int address, unsigned int size);
    void clear(unsigned int start, unsigned int size);
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

    boolean isSwitchPresent();
    uint8_t getSwitchGPIO();
    uint8_t getSwitchSensitiveness();

    boolean debuggable();
    
    void saveDeviceName(String in);
    void saveVersion(String in);
    void saveLanguage(String in);
    void saveInterface(uint8_t in);
    void saveWiFiSSID(String in);
    void saveWiFiPassword(String in);
    void saveMQTTHost(String in);
    void saveMQTTPort(unsigned int in);
    void saveMQTTUser(String in);
    void saveMQTTPassword(String in);
    void saveMQTTTopic(String in);
    void saveMode(uint8_t in); /* Look at MODE_... defined */
    void saveTemperatureCorrection(float in);
    void saveTemperatureInterval(unsigned int in);
    void saveTemperatureSensorPresent(uint8_t in);
    void saveRelayState(uint8_t in);
    void saveRelayStateAfterPowerRestored(uint8_t in);
    void saveRelayStateAfterConnectionRestored(uint8_t in);

    void saveSwitchPresent(uint8_t in);
    void saveSwitchGPIO(uint8_t in);
    void saveSwitchSensitiveness(uint8_t in);

    void saveDebuggable(byte in);
    
    
    void erase();
};
#endif



