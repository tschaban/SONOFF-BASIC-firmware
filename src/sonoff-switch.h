/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-03-18 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_switch_h
#define _sonoff_switch_h

#include <Ticker.h>

class SonoffSwitch
{
  private:  
    Ticker switchTimer;
    boolean state;
    unsigned long counter = 0;
    uint8_t _gpio;
    uint8_t _type;
    
  public:
    SonoffSwitch(); 
    void init(uint8_t gpio, uint8_t type);
    boolean stateChange();
    boolean delay();   
};
#endif

