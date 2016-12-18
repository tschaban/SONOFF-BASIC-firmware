/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

/* Button pressed method. Short changes relay state, long reboot device */
void button() {
  if (!digitalRead(BUTTON)) {
    pressedCount++;
  }
  else {
    if (pressedCount > 1 && pressedCount <= 10) {
      if (digitalRead(RELAY)==LOW) {
          relayOn();
      } else {
          relayOff();
      }
    }
    else if (pressedCount>30 && pressedCount <= 60 ){
      toggleMode();
    }
  pressedCount = 0;
  }
}
