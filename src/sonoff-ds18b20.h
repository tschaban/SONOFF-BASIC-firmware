#ifndef _sonoff_ds18b20_h
#define _sonoff_ds18b20_h


#include <DallasTemperature.h>
#include <OneWire.h>
#include "sonoff-configuration.h"

#define SENSOR_DS18B20 14

class SonoffDS18B20 {
  private:  
    float previousTemperature = 0;   
      
  public:
    SonoffDS18B20();
    float get(); 
};
#endif
