/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_led_h
#define _sonoff_led_h

#include <Ticker.h>
#include "sonoff-configuration.h"

#define GPIO_LED 13

class SonoffLED
{
  public:
    SonoffLED();
    void on();
    void off();
    void blink(int t = 100);
    void startBlinking(float t);
    void stopBlinking();

  private:
    Ticker LEDTimer;

};
#endif
