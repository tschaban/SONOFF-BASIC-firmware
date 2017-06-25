/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_upgrade_h
#define _sonoff_upgrade_h


class SonoffFirmware {
  private:  
    void v032();
    void v040();
    void v050();
    void v061();
    void v070();
    void v089();
    void v090();
     
  public:
    SonoffFirmware();
    boolean upgraded();
    void update();

};
#endif

