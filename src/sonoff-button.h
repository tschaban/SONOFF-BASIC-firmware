/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_button_h
#define _sonoff_button_h

#include <Ticker.h>
#include "sonoff-configuration.h"

#define BUTTON 0


class SonoffButton
{
  private:  
    Ticker buttonTimer;
    unsigned long counter = 0;
    
  public:
    SonoffButton(); 
    void start();    
    void stop();
    void pressed();
    void reset();
    boolean isPressed();

    boolean accessPointTrigger();
    boolean configurationTrigger();
    boolean relayTrigger();

    
};
#endif
