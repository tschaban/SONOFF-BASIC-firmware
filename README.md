# Software for Sonoff switch

**Work in progress - testing 3 devices at home right now ;-)**

**ToDo**
* Software crashes if DS18B20 is not connected or wrongly connected and when Sonoff is configured to use it

**Status of implementation:**
* firmware update over the network - **done**
* sonoff configuration by the browser - **done**
* possibility to connect sensor ds18b20 with automatic detection - **done**
* relay controlled by MQTT messages - **done**
* temperature published by MQTT broker - **done**
* relay controlled manually by sonoff button - **done**

**Hints**
* 1 short button press toggles the relay
* during first start it goes to configuration mode automatically
* if there is no WiFi and/or MQTT Broker configured device will go automatically to configuration mode

**Device configuraton**
There are two options to configure Sonoff by the browser
* accessing configuration page over your WiFI network or
* connecting directly to Sonoff when it's working in Access Point Mode

Access Point is the only option if WiFi has not been configured yet. Once the WiFi is configured you can choose from above to options.
Usually Access Point mode is used only for a first time while installing the device. Once WiFi, MQTT is entered easier option to configure Sonoff is by just connecting to it via browser. You have to know IP address of your Sonoff.

Going to configuration modes:
* Mode 1: Configuring by connecting to Sonoff Access Point: press Sonoff button until led start blinking fast
* Mode 2: Configuring by connecting to Sonoff IP Address : press Sonoff button for 3-7 sec. LED will turn on and after few seconds it will start blinking fast. 

If the device is in Access Point mode
* find SONOFF_XXXXXX Wifi network
* connect to it, no password needed
* open http://192.168.5.1 in your browser

If the device is in configuration mode 2 (above)
* enter IP address of Sonoff in your browser - you can find it in your WiFi ruter

To exit configuration mode
* press Sonoff button for 3-7 sec or
* click Exit link in Sonoff Configuration panel

**What is possible to configure via browser at the moment**
* WiFi access
* MQTT Broker
* MQTT Topic
* Temperature sensor 

**MQTT Topics (they may change)** 

| Topic  | Inbound / Outbound | Message | Description |
|---|---|---|---|---| 
| /sonoff/XXXXXX/cmd | Inbound | ON | Sets relay to ON  | 
| /sonoff/XXXXXX/cmd | Inbound | OFF | Sets relay to OFF | 
| /sonoff/XXXXXX/cmd | Inbound | reportStatus | Requests state of a relay. Sonoff repays with /sonoff/*ID*/state topic | 
| /sonoff/XXXXXX/cmd | Inbound | reset |  Resets switch | 
| /sonoff/XXXXXX/cmd | Inbound | configurationMode | It reboots Sonoff to configuration mode over your WiFi network  | 
| /sonoff/XXXXXX/state | Outbound | ON | Sonoff  publishes this if relay is set to ON by MQTT or manually by pressing Sonoff button |
| /sonoff/XXXXXX/state | Outbound | OFF | Sonoff  publishes this if relay is set to OFF by MQTT or manually by pressing Sonoff button |
| /sonoff/XXXXXX/temperature | Outbound | Number | Sonoff switch sends temperature from DS18B20 sensor if it was changed between subsequent measures | 

Hint:
* /sonoff/XXXXXX/ can be manually set to something different in the configuration. 

**Installation**
* Working binary is available in firmware folder. Version: 0.3.1

First time upload. 
* You can upload it to Sonoff using NodeMCU Flasher https://github.com/nodemcu/nodemcu-flasher

Upload parameters:
* Baudrate: 115200
* Flash size: 1MB
* Flash speed: 40MHz
* SPI Mode: QIO
* Binary must be uploaded from 0x00000 address

Next uploads can be made over the network which means in the configuration mode you have an option to upgrade firmware.



**Resources**
* Project web page (in polish): [here](http://smart-house.adrian.czabanowski.com/projekt-firmware-do-sonoff/)
* Project status (in polish): [here](http://smart-house.adrian.czabanowski.com/forum/ogolne/alternatywny-firmware-do-przelacznika-sonoff-projekt/#post-70)
