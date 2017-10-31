/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-10-31 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_dht_h
#define _sonoff_dht_h

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2 // Pin which is connected to the DHT sensor.

//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

class SonoffDHT {
private:
  float previousTemperature = 0;
  float previousHumidity = 0;
  sensors_event_t event;

public:
  SonoffDHT();
  float getTemperature();
  float getHumidity();
};
#endif
