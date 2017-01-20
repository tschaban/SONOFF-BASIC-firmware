#ifndef _sonoff_relay_h
#define _sonoff_relay_h

#include <PubSubClient.h>
#include "sonoff-eeprom.h"
#include "sonoff-led.h"
#include "sonoff-configuration.h"

#define RELAY 12


class SonoffRelay
{
  private:
    SonoffLED *_led;
    SonoffEEPROM *_eeprom;
    PubSubClient *_mqtt;
    
  public:
    SonoffRelay();
    void setup(SonoffLED *led, SonoffEEPROM *eeprom, PubSubClient *mqtt);
    void on();
    void off();
    void togle();
    void publish();
};
#endif
