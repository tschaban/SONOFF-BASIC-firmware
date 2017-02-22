/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-ds18b20.h"

SonoffDS18B20::SonoffDS18B20() {
  DS18B20.begin();
}

float SonoffDS18B20::get() {
  float temperature;  
  Serial << endl << "Requesting temperature" << endl;
  do {
    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));
  Serial << endl << " - temperature: " << temperature << endl;
  return temperature + Eeprom.DS18B20Correction();
}
