/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_interface_mqtt_h
#define _sonoff_interface_mqtt_h

#include <PubSubClient.h>

class SonoffMQTTInterface {
  private:
    PubSubClient Broker;

  public:
    SonoffMQTTInterface();
    void begin();
    void connect();
    void publish(char* type, char* message);
    boolean connected();
    void loop();
    void publishRelayState();

};
#endif
