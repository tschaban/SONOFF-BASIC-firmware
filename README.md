# Firmware for [Sonoff Basic switch](http://sonoff.itead.cc/en/products/sonoff/sonoff-basic)

**Resources**
* **PL** Dokumentacja w języku polskim: [tutaj](http://smart-house.adrian.czabanowski.com/firmware-sonoff/)
* **EN** Documentation in english: [here] (http://smart-house.adrian.czabanowski.com/en-sonoff-firmware/)

Documentation has not been updated to version 0.6 yet

**Binaries**
* [Latest binary version; ready to upload to a Sonoff Basic switch](https://github.com/tschaban/SONOFF-Firmwares/tree/master/BASIC/)


# Change log

**0.6.0 (2017-02-24)**
* Added possibility to define device name which is also used as WiFi SSID.
* Added option how Sonoff should behave after connection to MQTT is restored. Basically what should be the state of relay when Sonoff connects to MQTT. Possible options: ON,OFF,Last known,Server value
* Possible options of relay behaviour after Sonoff power is restored changed to : ON,OFF,Last known

**0.5.0 (2017-01-28)**
* Configuration panel UI updated
* Added PL language
* Refactoring methods related to ds18b20 sensor

**0.4.0 (2017-01-22)**
* Added option how Sonoff should behave after power is restored. Basically what should be the state of relay when Sonoff powers up. Possible options: ON,OFF,Last known,Server value
* some code refactoring

**0.3.2 (2017-01-15)**
* changed how LED indicates entry to the Configuration panel
* some code refactoring

**0.3.1 (2017-01-07)**
* first release