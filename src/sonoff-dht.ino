/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-10-31 tschaban https://github.com/tschaban
*/

#include "sonoff-dht.h"

SonoffDHT::SonoffDHT() { dht.begin(); }

float SonoffDHT::getTemperature() {
  float temperature;
  if (Configuration.debugger)
    Serial << endl << "INFO: Requesting temperature";
  do {
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(event.temperature);
      Serial.println(" *C");
      temperature = event.temperature;
    }
  } while (temperature == 85.0 || temperature == (-127.0));
  if (Configuration.debugger)
    Serial << endl << "INFO: temperature: " << temperature;
  return temperature; // + Eeprom.DS18B20Correction();
}

float SonoffDHT::getHumidity() {
  float humidity;
  if (Configuration.debugger)
    Serial << endl << "INFO: Requesting Humidity";
  do {
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity!");
    } else {
      Serial.print("Humidity: ");
      Serial.print(event.relative_humidity);
      Serial.println("%");
      humidity = event.relative_humidity;
    }
  } while (humidity == 85.0 || humidity == (-127.0));
  if (Configuration.debugger)
    Serial << endl << "INFO: Humidity: " << humidity;
  return humidity; // + Eeprom.DS18B20Correction();
}
