/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_core_h
#define _sonoff_core_h


#include <Ticker.h>
#include "sonoff-ds18b20.h"
#include "sonoff-upgrade.h"

class Sonoff
{
   private:  
    Ticker temperatureTimer;
        
    void runSwitch();
    void runConfigurationLAN();
    void runConfigurationAP();
 
    boolean isConfigured();
    void postUpgradeCheck();

    void setRelayAfterConnectingToMQTT();
    
    float previousTemperature = 0;
    
   
  public:
    Sonoff();
    void connectWiFi();
    void connectMQTT();
    void run();     
    void reset();
    void toggle();
    void listener();
    void setDS18B20Interval(unsigned int interval);
    void publishTemperature(float temperatur);


};
#endif

