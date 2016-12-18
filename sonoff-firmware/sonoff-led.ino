/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

/* Blink LED, t defines for how long LED should be ON */
void blinkLED(int t) {
  if (digitalRead(LED)==HIGH) {
    digitalWrite(LED, LOW);
  }
  delay(t);
  digitalWrite(LED, HIGH);
}
