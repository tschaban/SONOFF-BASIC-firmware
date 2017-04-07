/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/


void startHTTPInterface() {
  if (Configuration.debugger) Serial << endl << " - Starting web server" << endl;
  server.on("/", handleHTTPInterfaceCommand);
  server.on("/favicon.ico",handleFavicon);
  server.onNotFound(handleNotFound);
  server.begin();
  if (Configuration.debugger) Serial << " - Web server is working" << endl;
}

void handleHTTPInterfaceCommand() {
  String _status = "{'status':'success'}";
  
  if (server.args()==1) {
    if (server.arg(0)=="on") {
      Relay.on();
    } else if (server.arg(0)=="off") {
      Relay.off();
    } else if (server.arg(0)=="configurationMode") {
      Sonoff.toggle();
    } else if (server.arg(0)=="reboot") {
      ESP.restart();
    } else {
      _status = "{'status':'not implemented'}";
    }   
  } else {
    _status = "{'status':'failure'}";
  }

  server.send(200, "text/json", _status);
}

