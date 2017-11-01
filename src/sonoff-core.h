/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_core_h
#define _sonoff_core_h

#include "sonoff-dht.h"
//#include "sonoff-ds18b20.h"
#include "sonoff-upgrade.h"
#include <Ticker.h>

class Sonoff {
private:
  unsigned long temperatureTimer = 0;
  unsigned long humidityTimer = 0;
  Ticker sleepModeTimer;

  void runSwitch();
  void runConfigurationLAN();
  void runConfigurationAP();

  boolean isConfigured();
  void postUpgradeCheck();

  void setRelayAfterConnectingToMQTT();

  float previousTemperature = 0;
  float previousHumidity = 0;

public:
  Sonoff();
  void connectWiFi();
  void connectMQTT();
  void run();
  void reset();
  void toggle();
  void listener();
  // void setDS18B20Interval(unsigned int interval);
  void setDHTInterval(unsigned int interval);
  void publishTemperature(float temperatur);
  void publishHumidity(float humidity);

  void runSleepMode();
  void stopSleepMode();
};
#endif
