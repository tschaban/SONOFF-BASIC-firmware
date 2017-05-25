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

  Button.stop();  // Buton listener is stopped for the time of state update resquest to Domoticz. It's turned on upon sucessful request.

  if (Configuration.debugger) Serial << endl << "INFO: Received request to publish state to Domoticz ";

  // Construction of relay state urls
  if (in == RELAY_ON) {
    url = "GET /json.htm?type=command&param=switchlight&idx=" + String(idx) + "&switchcmd=On HTTP/1.1";
  } else {
    url = "GET /json.htm?type=command&param=switchlight&idx=" + String(idx) + "&switchcmd=Off HTTP/1.1" ;
  }

  doRequest = true; // Turns on mechanism of url calling. It's invoked from Sonoff.listener method.

  if (Configuration.debugger) Serial << endl << "INFO: " << server << ":" << port << " " << url;
  if (Configuration.debugger) Serial << endl << "INFO: Publishing";

}

void SonoffDomoticzInterface::pushRequest() {
  if (doRequest) { // Skips method execution if mechanism of url calling is off..
    if (Configuration.debugger) Serial << ".";
    if (esp.connect(server, port)) {
      esp.println(url);
      esp.print("Host: ");
      esp.println(server);
      esp.println("Connection: close");
      esp.println();
      if (Configuration.debugger) Serial << endl << "INFO: Request to Domoticz has been sent ";
      doRequest = false; // Turns off mechanism of url calling
      Button.start(); // Enables button listener
    }
  }
}





