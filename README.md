# Software for Sonoff switch

**Work in progress - not ready for productive release yet, although core functions are working**

**Status of implementation:**
* firmware update over the network - **not started**
* sonoff configuration by the browser - **done, partially tested, additional options might be added**
* possibility to connect sensor ds18b20 with automatic detection - **in progress**
* relay controlled by MQTT messages - **done, tested - be aware that format of MQTT message may change**
* temperature published by MQTT broker - **done, tested - be aware that format of MQTT message may change**
* relay controlled manually by sonoff button - **done, unit tested**

**Hints**
* 1 short button press toggles the relay
* 3-4 sec. button press toggles device mode: configuration mode or switch mode
* if there is no WiFi and/or MQTT Broker configured device will go automatically to configuration mode
* in order configure device via browser: find SONOFF_xxxxxx WiFi network - connect to it (no password needed) and then open http://192.168.5.1 in your browser 

**What is possible to configure via browser at the moment**
* WiFi access
* MQTT Broker
* MQTT Topic
* Temperature sensor 

**MQTT Topics (they may change)** 

| Topic  | Inbound / Outbound | Message | Description |
|---|---|---|---|---| 
| /sonoff/*ID*/cmd | Inbound | turnON | Sets relay to ON  | 
| /sonoff/*ID*/cmd | Inbound | turnOFF | Sets relay to OFF | 
| /sonoff/*ID*/cmd | Inbound | reportStatus | Requests state of a relay. Sonoff repays with /sonoff/*ID*/state topic | 
| /sonoff/*ID*/cmd | Inbound | reset |  Resets switch | 
| /sonoff/*ID*/cmd | Inbound | tempInterval:N |  **NOT IMPLEMENTED** Sets how often Sonoff should read value of DS18B20 sensor. N - number of seconds. Example: tempInterval:60 - means read sesonr value every 1 minute. Default: 10min  | 
| /sonoff/*ID*/cmd | Inbound | tempCorrection:N | **NOT IMPLEMENTED**  Sets a value of temperature correction. Example tempCorrection:-1.2 - means correct returned temperature by -1.2. Default: 0 | 
| /sonoff/*ID*/state | Outbound | ON | Sonoff  publishes this if relay is set to ON by MQTT or manually by pressing Sonoff button |
| /sonoff/*ID*/state | Outbound | OFF | Sonoff  publishes this if relay is set to OFF by MQTT or manually by pressing Sonoff button |
| /sonoff/*ID*/get | Outbound | defaultState | Sonoff switch sends this message to the broker while booting in order to get default values of the relay, temp.correction and interval of sensor read. If it's not implemented in the MQTT broker then default values are set | 
| /sonoff/*ID*/temperature | Outbound | Number | Sonoff switch sends temperature from DS18B20 sensor if it was changed between subsequent measures | 

where 
*  _ID_ is a value of particular switch ChipID - it could be set manually to whatever value in the configuration part of a sketch **Planning to implement set it up via browser as well**

**Resources**
* Project web page (in polish): [here](http://smart-house.adrian.czabanowski.com/projekt-firmware-do-sonoff/)
* Project status (in polish): [here](http://smart-house.adrian.czabanowski.com/forum/oprogramowanie/alternatywny-firmware-do-przelacznika-sonoff-projekt/)
