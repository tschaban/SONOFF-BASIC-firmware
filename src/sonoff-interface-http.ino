/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-interface-http.h"

SonoffHTTPInterface::SonoffHTTPInterface() {}

void SonoffHTTPInterface::begin() {
  if (Configuration.debugger) Serial << endl << "INFO: Starting web server";
  server.on("/", handeHTTPInterfaceRequests);
  server.on("/favicon.ico", handleFavicon);
  server.onNotFound(handleNotFound);
  server.begin();
  if (Configuration.debugger) Serial << endl << "INFO: Web server ready";
}

void handeHTTPInterfaceRequests() {
  String _status = "{'status':'success'}";

  if (Configuration.debugger) Serial << endl << "INFO: Got HTTP request";

  if (server.args() == 1) {
    if (Configuration.debugger) Serial << endl << "INFO: HTTP Argument=" << server.arg(0);
    if (server.arg(0) == "on") {
      Relay.on();
    } else if (server.arg(0) == "off") {
      Relay.off();
    } else if (server.arg(0) == "configurationMode") {
      Sonoff.toggle();
    } else if (server.arg(0) == "reboot") {
      ESP.restart();
    } else {
      _status = "{'status':'not implemented'}";
    }
  } else {
    _status = "{'status':'failure'}";
  }

  server.send(200, "text/json", _status);
}

