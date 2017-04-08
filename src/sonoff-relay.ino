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
  
  if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_ON && get()==RELAY_OFF) {
    digitalWrite(RELAY, HIGH);
  } else if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_OFF && get()==RELAY_ON) {
    digitalWrite(RELAY, LOW);
  } else if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_LAST_KNOWN) {
    if (Eeprom.getRelayState()==RELAY_ON  && get()==RELAY_OFF) {
      digitalWrite(RELAY, HIGH);
    } else if (Eeprom.getRelayState()==RELAY_OFF  && get()==RELAY_ON) {
      digitalWrite(RELAY, LOW);
    }
  }
}

uint8_t SonoffRelay::get() {
  return digitalRead(RELAY)==HIGH?RELAY_ON:RELAY_OFF;
}

/* Set relay to ON */
void SonoffRelay::on() {
  if (get()==RELAY_OFF) digitalWrite(RELAY, HIGH);
  if (Configuration.interface == 1) publish();
  if (Configuration.debugger) Serial << endl << "INFO: Relay set to ON";
  Eeprom.saveRelayState(RELAY_ON);
  Led.blink();
}

/* Set relay to OFF */
void SonoffRelay::off() {
  if (get()==RELAY_ON) digitalWrite(RELAY, LOW);
  if (Configuration.interface == 1) publish();
  if (Configuration.debugger) Serial << endl << "INFO: Relay set to OFF";
  Eeprom.saveRelayState(RELAY_OFF);
  Led.blink();
}

/* Toggle relay */
void SonoffRelay::toggle() {
  if (digitalRead(RELAY) == LOW) {
    on();
  } else {
    off();
  }
}

void SonoffRelay::publish() {
  if (digitalRead(RELAY) == LOW) {
    Mqtt.publish((char*)"state", (char*)"OFF");
  } else {
    Mqtt.publish((char*)"state", (char*)"ON");
  }
}


