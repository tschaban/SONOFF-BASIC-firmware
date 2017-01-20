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
    void stop();
    void pressed();
    void reset();
    boolean isPressed();

    boolean accessPointTrigger();
    boolean configurationTrigger();
    boolean relayTrigger();

    
};
#endif
