/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-relay.h"

SonoffRelay::SonoffRelay() {
  pinMode(RELAY, OUTPUT);
}

void SonoffRelay::setup(SonoffLED *led, SonoffEEPROM *eeprom, PubSubClient *mqtt) {
  _led = led;
  _eeprom = eeprom;
  _mqtt = mqtt;

  Serial << endl << "Setting relay default " << endl;

  if (_eeprom->getRelayState() == 1) {
    digitalWrite(RELAY, HIGH);
  } else {
    digitalWrite(RELAY, LOW);
  }
}

/* Set relay to ON */
void SonoffRelay::on() {
  digitalWrite(RELAY, HIGH);
  publish();
  Serial << "Relay set to ON" << endl;
  _eeprom->saveRelayState(1);
  _led->blink();
}

/* Set relay to OFF */
void SonoffRelay::off() {
  digitalWrite(RELAY, LOW);
  publish();
  Serial << "Relay set to OFF" << endl;
  _eeprom->saveRelayState(0);
  _led->blink();
}

/* Toggle relay */
void SonoffRelay::togle() {

  if (digitalRead(RELAY) == LOW) {
    on();
  } else {
    off();
  }
}

void SonoffRelay::publish() {

  char  mqttString[50];
  sprintf(mqttString, "%sstate", _eeprom->getMQTTTopic());
  if (digitalRead(RELAY) == LOW) {
    _mqtt->publish(mqttString, "OFF");
  } else {
    _mqtt->publish(mqttString, "ON");
  }
}

