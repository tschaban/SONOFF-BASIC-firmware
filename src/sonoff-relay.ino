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

/* Set relay to ON */
void SonoffRelay::on() {
  digitalWrite(RELAY, HIGH);
  publish();
  Serial << "Relay set to ON" << endl;
  Eeprom.saveRelayState(1);
  Led.blink();
}

/* Set relay to OFF */
void SonoffRelay::off() {
  digitalWrite(RELAY, LOW);
  publish();
  Serial << "Relay set to OFF" << endl;
  Eeprom.saveRelayState(0);
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
  char  mqttString[50];
  sprintf(mqttString, "%sstate", Configuration.mqtt_topic);
  if (digitalRead(RELAY) == LOW) {
    Mqtt.publish(mqttString, "OFF");
  } else {
    Mqtt.publish(mqttString, "ON");
  }
}

