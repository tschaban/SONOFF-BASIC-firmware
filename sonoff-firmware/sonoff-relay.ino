/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-relay.h"

SonoffRelay::SonoffRelay() {
  pinMode(RELAY, OUTPUT);

  /* Default value while booting sonoff */

  if (Eeprom.getRelayStateAfterPowerRestored() == DEFAULT_RELAY_ON && get() == RELAY_OFF) {
    digitalWrite(RELAY, HIGH);
  } else if (Eeprom.getRelayStateAfterPowerRestored() == DEFAULT_RELAY_OFF && get() == RELAY_ON) {
    digitalWrite(RELAY, LOW);
  } else if (Eeprom.getRelayStateAfterPowerRestored() == DEFAULT_RELAY_LAST_KNOWN) {
    if (Eeprom.getRelayState() == RELAY_ON  && get() == RELAY_OFF) {
      digitalWrite(RELAY, HIGH);
    } else if (Eeprom.getRelayState() == RELAY_OFF  && get() == RELAY_ON) {
      digitalWrite(RELAY, LOW);
    }
  }
}

uint8_t SonoffRelay::get() {
  return digitalRead(RELAY) == HIGH ? RELAY_ON : RELAY_OFF;
}

/* Set relay to ON */
void SonoffRelay::on() {
  if (get() == RELAY_OFF) {
    digitalWrite(RELAY, HIGH);
    Eeprom.saveRelayState(RELAY_ON);
    if (Configuration.debugger) Serial << endl << "INFO: Relay set to ON";
  }
  Led.blink();
}

/* Set relay to OFF */
void SonoffRelay::off() {
  if (Configuration.relay_auto_off_time > 0) autoOffTimer.detach(); /* just in case - disable auto-off timer (but bother only if time is defined)*/

  if (get() == RELAY_ON) {
    digitalWrite(RELAY, LOW);
    Eeprom.saveRelayState(RELAY_OFF);
    if (Configuration.debugger) Serial << endl << "INFO: Relay set to OFF";
  }
  Led.blink();
}

/* Toggle relay */
void SonoffRelay::toggle() {
  if (digitalRead(RELAY) == LOW) {
    on();
    if (Configuration.relay_auto_off_time > 0) {
        if (Configuration.debugger) Serial << endl << "INFO: Scheduling auto-OFF in " << Configuration.relay_auto_off_time << "s";
        autoOffTimer.once((float)Configuration.relay_auto_off_time, callbackRelayAutoOff);
    }
  } else {
    off();
  }
}


void callbackRelayAutoOff() {
    if (Configuration.debugger) Serial << endl << "INFO: Auto OFF kicks in";
    Relay.off();
}
