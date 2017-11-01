/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-10-31 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_dht_h
#define _sonoff_dht_h

#include <DHT.h>

class SonoffDHT {
private:
  float previousTemperature = 0;
  float previousHumidity = 0;

public:
  SonoffDHT();
  float getTemperature();
  float getHumidity();
};
#endif
