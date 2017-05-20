/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2017-05-19 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_interface_domoticz_h
#define _sonoff_interface_domoticz_h

class SonoffDomoticzInterface {
  private:
    char*         server;
    unsigned int  port;
    unsigned int  idx;

    String url;
       
    bool doRequest = false;
      
  public:
    SonoffDomoticzInterface();
    void begin();
    void publishRelayState(uint8_t in);
    void pushRequest();
};
#endif

