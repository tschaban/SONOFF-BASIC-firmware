# Firmware for [Sonoff Basic switch](http://sonoff.itead.cc/en/products/sonoff/sonoff-basic)

**Resources**
* **EN** Documentation in english: [here](http://smart-house.adrian.czabanowski.com/en-sonoff-firmware/)
* **PL** Dokumentacja w języku polskim: [tutaj](http://smart-house.adrian.czabanowski.com/firmware-sonoff/)

**Binaries**
* [Latest binary version; ready to upload to a Sonoff Basic switch](https://github.com/tschaban/SONOFF-Firmwares/tree/master/BASIC/)


# Change log
**0.10dh (2017-11-01)**
* This version supports temperature and humidity sensors DH11, DH21, DH22
* Support of DS18B20 sensor is removed from this version
* IMPORTANT: if you are not going to use DHxx sensor or you need DS18b20 sensor then install version 0.8.9



**0.8.9 (2017-06-08)**
* Added option to select the type of external switch (mono- or bistable) on configuration screen
* Added option to publish relay status to Domoticz (authorization is not implemented yet)
* It's possible to request relay state by HTTP request http://ip-sonoff/?cmd=reportStatus

**0.8.1 (2017-04-29)**
* Defect fix: DS18B20 not launching
* Defect fix: Debugger flag was not saved

**0.8.0 (2017-04-28)**
* Added possibility to control Sonoff with HTTP protocol
* Added possibility to turn off Sonoff control by MQTT or HTTP protocol. Manual control is possible
* Changed the name of Exit link to Exit configuration
* MQTT comment to reboot Sonoff changed from reset to reboot
* Defect fix: Wrong error formatting while upgrading sonoff with failure
* Defect fix: relay value not request each time when connection established to MQTT Broker
* Changed mechanism for WiFi and MQTT connection establishing. It increases Sonoff stability
* Couple of small UI changes 


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