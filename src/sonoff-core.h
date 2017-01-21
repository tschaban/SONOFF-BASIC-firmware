#ifndef _sonoff_core_h
#define _sonoff_core_h


#include <Ticker.h>
#include "sonoff-configuration.h"
#include "sonoff-ds18b20.h"
#include "sonoff-led.h"
#include "sonoff-eeprom.h"


class Sonoff
{
   private:  
    Ticker temperatureTimer;
    
    void runSwitch();
    void runConfigurationLAN();
    void runConfigurationAP();
    boolean isConfigured();
    void postUpgradeCheck();
    float previousTemperature = 0;
    
   
  public:
    Sonoff();
    void connectWiFi();
    void connectMQTT();
    void run();     
    void reset();
    void toggle();
    void listener();
    void setDS18B20Interval(int interval);
    void publishTemperature(float temperatur);


};
#endif
