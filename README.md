# Firmware for [Sonoff Basic switch](http://sonoff.itead.cc/en/products/sonoff/sonoff-basic)

**Resources**
* **EN** Documentation in english: [here](http://smart-house.adrian.czabanowski.com/en-sonoff-firmware/)
* **PL** Dokumentacja w języku polskim: [tutaj](http://smart-house.adrian.czabanowski.com/firmware-sonoff/)

**Binaries**
* [Latest binary version; ready to upload to a Sonoff Basic switch](https://github.com/tschaban/SONOFF-Firmwares/tree/master/BASIC/)


# Change log

**0.7.0 (2017-03-18)**
* Added possibility to control sonoff with external switch
* Added option tu turn on/off serial debugger
* Fix defect related to host name broadcasting as an access point in local network

**0.6.1 (2017-02-25)**
* Device name is also set as host name in local network. (keep in mind not to use space while naming it)

**0.6.0 (2017-02-24)**
* Added possibility to define device name which is also used as WiFi SSID.
* Added option how Sonoff should behave after connection to MQTT is restored. Basically what should be the state of the relay when Sonoff connects to MQTT. Possible options: ON,OFF,Last known,Server value
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