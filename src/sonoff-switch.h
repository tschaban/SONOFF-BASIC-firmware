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
    
  public:
    SonoffSwitch(); 
    void init(uint8_t gpio);
    boolean stateChange();
    boolean delay();   
};
#endif

