[PL: Opis zmian w jeżyku polskim](http://smart-house.adrian.czabanowski.com/forum/firmware-do-przelacznika-sonoff-ogolne/sonoff-firmware-log-zmian/#post-76)

**0.4.0 (2017-01-22)**
* Added option how Sonoff should behave after power interruption. Basically what should be the state of relay when Sonoff powers up. Possible options: ON,OFF,Last known,Server value
* some code refactoring

Be aware before upgrading:
* This version has not been well tested by me yet
* I had some issues with one of my switches while upgrading from 0.3.2 to 0.4.0. Sonoff couldn't connect to MQTT. Downgrade to 0.3.2 and upgrade to 0.4.0 solved it
 
**0.3.2 (2017-01-15)**
* changed how LED indicates entry to the Configuration panel
* some code refactoring

**0.3.1 (2017-01-07)**
* first release