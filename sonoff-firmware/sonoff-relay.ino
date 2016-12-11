
/* Set relay to ON */
void relayOn() {
  digitalWrite(RELAY, HIGH);
  publishRelayStateMessage();
}

/* Set relay to OFF */
void relayOff() {
  digitalWrite(RELAY, LOW);
  publishRelayStateMessage();
}

/* Toggle relay */
void relayToggle() {
  if (digitalRead(RELAY) == LOW) {
    relayOn();
  } else {
    relayOff();
  }
}
