/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

unsigned long pressedCount = 0;

/* Button pressed method. Short changes relay state, long reboot device */
void callbackButton() {

  if (!digitalRead(BUTTON) && pressedCount < 80) {
    pressedCount++;
  } else if (!digitalRead(BUTTON) && pressedCount == 80) {
      buttonTimer.detach();
      if (sonoffConfig.mode==2) {
        runSwitchMode();
      } else {
         configuratonAPMode();
      }
  } else {
    if (sonoffConfig.mode==0 && pressedCount > 1 && pressedCount <= 10) {
      if (digitalRead(RELAY)==LOW) {
          Relay.on();
      } else {
          Relay.off();
      }
    } else if (pressedCount>20 && pressedCount < 80 ){
      toggleMode();
    } 
  pressedCount = 0;
  }
}
