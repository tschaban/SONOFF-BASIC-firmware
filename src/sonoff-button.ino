/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-button.h"


SonoffButton::SonoffButton() {
  pinMode(BUTTON, INPUT_PULLUP);
  buttonTimer.attach(0.05, callbackButton);
}

void SonoffButton::stop() {
  buttonTimer.detach();
}

boolean SonoffButton::isPressed() {
  return !digitalRead(BUTTON);
}

void SonoffButton::pressed() {
  counter++;
}

void SonoffButton::reset() {
  counter = 0;
}

boolean SonoffButton::accessPointTrigger() {
   return counter==80?true:false;
}

boolean SonoffButton::configurationTrigger() {
  return counter>20&&counter<80?true:false;

}

boolean SonoffButton::relayTrigger() {
  return counter>1&&counter<=10?true:false;
}

void callbackButton() {

  if (Button.isPressed() && !Button.accessPointTrigger()) {
    Button.pressed();
  } else if (Button.isPressed() && Button.accessPointTrigger()) {
    Button.stop();
    if (Configuration.mode==MODE_ACCESSPOINT) {
      Sonoff.run();
    } else {
       Eeprom.saveMode(MODE_ACCESSPOINT);
       delay(10);
       ESP.restart();
    }
  } else {
    if (Configuration.mode==MODE_SWITCH && Button.relayTrigger()) {
      Relay.toggle();
    } else if (Button.configurationTrigger()) {
      Sonoff.toggle();
    }
    Button.reset();
  }
}
