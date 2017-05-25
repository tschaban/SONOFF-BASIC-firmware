/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-led.h"

SonoffLED::SonoffLED() {
  pinMode(GPIO_LED, OUTPUT);
  digitalWrite(GPIO_LED, HIGH);
}

void SonoffLED::on() {
  if (digitalRead(GPIO_LED) == HIGH) {
    digitalWrite(GPIO_LED, LOW);
  }
}

void SonoffLED::off() {
  if (digitalRead(GPIO_LED) == LOW) {
    digitalWrite(GPIO_LED, HIGH);
  }
}

/* Blink GPIO_LED, t defines for how long GPIO_LED should be ON */
void SonoffLED::blink(unsigned int t) {
  on();
  delay(t);
  off();
}

void SonoffLED::startBlinking(float t) {
  LEDTimer.attach(t, callbackLED);
}

void SonoffLED::stopBlinking() {
  LEDTimer.detach();
}


void callbackLED() {
  if (digitalRead(GPIO_LED) == HIGH) {
    digitalWrite(GPIO_LED, LOW);
  } else {
    digitalWrite(GPIO_LED, HIGH);
  }
}

