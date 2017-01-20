/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
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
    if (eeprom.getMode()==MODE_ACCESSPOINT) {
      runSwitchMode();
    } else {
      configuratonAPMode();
    }
  } else {
    if (eeprom.getMode() == 0 && Button.relayTrigger()) {
      Relay.togle();
    } else if (Button.configurationTrigger()) {
      toggleMode();
    }
    Button.reset();
  }
}
