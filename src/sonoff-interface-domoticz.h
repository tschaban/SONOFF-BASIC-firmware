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
    char*         server;   // Domoticz IP
    unsigned int  port;     // Domoticz port
    unsigned int  idx;      // Device IDX in Domoticz
    String url;             // State updatre url       
    bool doRequest = false; // Turns On/Off URL calling mechanism. It's turned ononly if url should be requested
       
      
  public:
    SonoffDomoticzInterface();
    void begin();
    void publishRelayState(uint8_t in);
    void pushRequest();     // Methods calls url from server:port when doRequest is true
    
};
#endif

