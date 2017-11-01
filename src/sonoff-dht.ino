/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-10-31 tschaban https://github.com/tschaban
*/

#include "sonoff-dht.h"

SonoffDHT::SonoffDHT() {}

float SonoffDHT::getTemperature() {

  DHT dht(14, Configuration.dht_type == 1
                  ? DHT11
                  : Configuration.dht_type == 2 ? DHT21 : DHT22);
  dht.begin();

  float temperature;
  if (Configuration.debugger)
    Serial << endl << "INFO: Requesting temperature";
  temperature = dht.readTemperature();
  if (Configuration.debugger) {
    if (isnan(temperature)) {
      Serial << endl << "WARN: Error reading DHT sensor temperature";
    } else {
      Serial << endl << "INFO: temperature: " << temperature;
    }
  }
  return temperature + Configuration.dht_temperature_correction;
}

float SonoffDHT::getHumidity() {

  DHT dht(14, Configuration.dht_type == 1
                  ? DHT11
                  : Configuration.dht_type == 2 ? DHT21 : DHT22);
  dht.begin();

  float humidity;
  if (Configuration.debugger)
    Serial << endl << "INFO: Requesting Humidity";
  humidity = dht.readHumidity();
  if (Configuration.debugger) {
    if (isnan(humidity)) {
      Serial << endl << "WARN: Error reading DHT sensor humidity";
    } else {
      Serial << endl << "INFO: humidity: " << humidity;
    }
  }
  return humidity;
  +Configuration.dht_humidity_correction;
}
