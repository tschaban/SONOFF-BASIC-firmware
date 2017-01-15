#ifndef _sonoff_relay_h
#define _sonoff_relay_h

#include <PubSubClient.h>
#include "sonoff-eeprom.h"
#include "sonoff-led.h"
#include "sonoff-configuration.h"

class SonoffRelay
{
  private:
    SONOFFCONFIG *_configuration;  
    SonoffLED *_led;
    SonoffEEPROM *_eeprom;
    PubSubClient *_mqtt;
    
  public:
    SonoffRelay(SONOFFCONFIG *configuration);
    void setup(SonoffLED *led, SonoffEEPROM *eeprom, PubSubClient *mqtt);
    void on();
    void off();
    void togle();
    void publish();
  
};
#endif
