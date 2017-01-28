/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <DallasTemperature.h>
#include <OneWire.h>


#include "Streaming.h"
#include "sonoff-configuration.h"
#include "sonoff-core.h"
#include "sonoff-led.h"
#include "sonoff-eeprom.h"
#include "sonoff-relay.h"
#include "sonoff-button.h"
#include "sonoff-ota.h"

DEFAULTS sonoffDefault;
SONOFFCONFIG Configuration; 

WiFiClient esp;
PubSubClient Mqtt(esp);

ESP8266WebServer server(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;
OneWire wireProtocol(SENSOR_DS18B20);
DallasTemperature DS18B20(&wireProtocol);

SonoffEEPROM    Eeprom;
SonoffRelay     Relay;
SonoffButton    Button;
SonoffLED       Led;
Sonoff          Sonoff;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();

  Configuration = Eeprom.getConfiguration();

 
  Serial << endl << "Configuration: " << endl;
  Serial << " - Version: " << Configuration.version << endl;
  Serial << " - Language: " << Configuration.language << endl;
  Serial << " - Switch mode: " << Configuration.mode << endl;
  Serial << " - Device ID: " << Configuration.id << endl;
  Serial << " - Host name: " << Configuration.host_name << endl;
  Serial << " - WiFi SSID: " << Configuration.wifi_ssid << endl;
  Serial << " - WiFi Password: " << Configuration.wifi_password << endl;
  Serial << " - MQTT Host: " << Configuration.mqtt_host << endl;
  Serial << " - MQTT Port: " << Configuration.mqtt_port << endl;
  Serial << " - MQTT User: " << Configuration.mqtt_user << endl;
  Serial << " - MQTT Password: " << Configuration.mqtt_password << endl;
  Serial << " - MQTT Topic: " << Configuration.mqtt_topic <<  endl;
  Serial << " - DS18B20 present: " << Configuration.ds18b20_present << endl;
  Serial << " - Temp correctin: " << Configuration.ds18b20_correction << endl;
  Serial << " - Temp interval: " << Configuration.ds18b20_interval << endl;
  Serial << " - Relay state: " << Eeprom.getRelayState() << endl;
  Serial << " - Post crash relay state: " << Eeprom.getRelayStartState() << endl;

 
  Sonoff.run(); 
  
}

void loop() {
  Sonoff.listener();
}
