/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2017-05-19 tschaban https://github.com/tschaban
*/

#include "sonoff-interface-domoticz.h"


SonoffDomoticzInterface::SonoffDomoticzInterface() {}

void SonoffDomoticzInterface::begin() {
  server = Configuration.mqtt_host;
  port = Configuration.mqtt_port;
  idx = Configuration.domoticz_idx;

}

void SonoffDomoticzInterface::publishRelayState(uint8_t in) {
  if (Configuration.debugger) Serial << endl << "INFO: Received request to publish state to Domoticz ";  
  if (in == RELAY_ON) {
    url = "GET /json.htm?type=command&param=switchlight&idx=1&switchcmd=On HTTP/1.1";
  } else {
    url = "GET /json.htm?type=command&param=switchlight&idx=1&switchcmd=Off HTTP/1.1" ;
  }
  doRequest = true;
  if (Configuration.debugger) Serial << endl << "INFO: " << server << ":" << port << " " << url;
  if (Configuration.debugger) Serial << endl << "INFO: Publishing";
  Button.stop();    
}

void SonoffDomoticzInterface::pushRequest() {
  if (doRequest) {
    if (Configuration.debugger) Serial << "."; 
    if (esp.connect(server, port)) {      
      esp.println(url);
      esp.print("Host: ");
      esp.println(server);
      esp.println("Connection: close");
      esp.println();
      if (Configuration.debugger) Serial << endl << "INFO: Sent ";
      doRequest = false;
      Button.start();
    }
  }
}





