
#include "sonoff-ds18b20.h"

SonoffDS18B20::SonoffDS18B20() {
}

float SonoffDS18B20::get() {
  float temperature;
  OneWire wireProtocol(SENSOR_DS18B20);
  DallasTemperature DS18B20(&wireProtocol);
  DS18B20.begin();
  Serial << endl << "Requesting temperature" << endl;
  do {
    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));
  Serial << endl << " - temperature: " << temperature << endl;
  return temperature + Eeprom.DS18B20Correction();
}
