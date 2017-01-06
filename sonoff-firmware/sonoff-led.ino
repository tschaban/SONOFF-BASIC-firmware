/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/
void LEDOn() {
  if (digitalRead(LED) == HIGH) {
    digitalWrite(LED, LOW);
  }
}

void LEDOff() {
  if (digitalRead(LED) == LOW) {
    digitalWrite(LED, HIGH);
  }

}

/* Blink LED, t defines for how long LED should be ON */
void blinkLED(int t) {
  LEDOn();
  delay(t);
  LEDOff();
}

void blinkLEDInLoop(float t) {
  configurationMode.attach(t, callbackLED);
}

void stopBlinkLEDInLoop() {
  configurationMode.detach();
}


void callbackLED() {
  if (digitalRead(LED) == HIGH) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
}
