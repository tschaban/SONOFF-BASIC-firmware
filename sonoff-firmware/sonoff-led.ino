/* Blink LED, t defines for how long LED should be ON */
void blinkLED(int t) {
  if (digitalRead(LED)==HIGH) {
    digitalWrite(LED, LOW);
  }
  delay(t);
  digitalWrite(LED, HIGH);
}
