/* Button pressed method. Short changes relay state, long reboot device */
void button() {
  if (!digitalRead(BUTTON)) {
    pressedCount++;
  }
  else {
    if (pressedCount > 1 && pressedCount <= 50) {
      if (digitalRead(RELAY)==LOW) {
          relayOn();
      } else {
          relayOff();
      }
    }
    else if (pressedCount >50){
      blinkLED();
    }
  pressedCount = 0;
  }
}
