/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include <DHT.h>
#include <DHT_U.h>
#include <DNSServer.h>
#include <DallasTemperature.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include <OneWire.h>

#include "Streaming.h"
#include "sonoff-button.h"
#include "sonoff-configuration.h"
#include "sonoff-core.h"
#include "sonoff-dht.h"
#include "sonoff-eeprom.h"
#include "sonoff-interface-domoticz.h"
#include "sonoff-interface-http.h"
#include "sonoff-interface-mqtt.h"
#include "sonoff-led.h"
#include "sonoff-ota.h"
#include "sonoff-relay.h"
#include "sonoff-switch.h"

DEFAULTS sonoffDefault;
SONOFFCONFIG Configuration;

WiFiClient esp;

ESP8266WebServer server(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;
OneWire wireProtocol(SENSOR_DS18B20);
DallasTemperature DS18B20(&wireProtocol);

DHT_Unified dht(DHTPIN, DHTTYPE);

SonoffEEPROM Eeprom;
SonoffRelay Relay;
SonoffButton Button;
SonoffSwitch Switch;
SonoffLED Led;
SonoffMQTTInterface MqttInterface;
SonoffHTTPInterface HttpInterface;
SonoffDomoticzInterface DomoticzInterface;
Sonoff Sonoff;

void setup() {

  Configuration = Eeprom.getConfiguration();

  if (Configuration.debugger) {
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial << endl << "INFO: Configuration: " << endl;
    Serial << " - Version: " << Configuration.version << endl;
    Serial << " - Language: " << Configuration.language << endl;
    Serial << " - Switch mode: " << Configuration.mode << endl;
    Serial << " - Interface: " << Configuration.interface << endl;
    Serial << "   - Host: " << Configuration.mqtt_host << endl;
    Serial << " - Device ID: " << Configuration.id << endl;
    Serial << " - Host name: " << Configuration.device_name << endl;
    Serial << " - WiFi:" << endl;
    Serial << "   - SSID: " << Configuration.wifi_ssid << endl;
    Serial << "   - Password: " << Configuration.wifi_password << endl;
    Serial << " - MQTT or Domoticz: " << endl;
    Serial << "   - Port: " << Configuration.mqtt_port << endl;
    Serial << "   - User: " << Configuration.mqtt_user << endl;
    Serial << "   - Password: " << Configuration.mqtt_password << endl;
    Serial << "   - MQTT Topic: " << Configuration.mqtt_topic << endl;
    Serial << "   - DomoticzIDX:  " << Configuration.domoticz_idx << endl;
    Serial << " - Connections: " << endl;
    Serial << "   - Number connection attempts: "
           << Configuration.number_connection_attempts << endl;
    Serial << "   - Duration between connection attempts [sec]: "
           << Configuration.duration_between_connection_attempts << endl;
    Serial << "   - Duration between next connection series [min]: "
           << Configuration.duration_between_next_connection_attempts_series
           << endl;
    Serial << " - DS18B20 Sensor: " << endl;
    Serial << "   - Present: " << Configuration.ds18b20_present << endl;
    Serial << "   - Temp correctin: " << Configuration.ds18b20_correction
           << endl;
    Serial << "   - Temp interval: " << Configuration.ds18b20_interval << endl;
    Serial << " - External switch " << endl;
    Serial << "   - Present: " << Configuration.switch_present << endl;
    Serial << "   - GPIO: " << Configuration.switch_gpio << endl;
    Serial << "   - Type: " << Configuration.switch_type << endl;
    Serial << "   - Sensitivness: " << Configuration.switch_sensitiveness
           << endl;
    Serial << " - Relay " << endl;
    Serial << "   - State: " << Eeprom.getRelayState() << endl;
    Serial << "   - Post power restore : "
           << Eeprom.getRelayStateAfterPowerRestored() << endl;
    Serial << "   - Post reconnection to MQTT : "
           << Eeprom.getRelayStateAfterConnectionRestored() << endl;
    Serial << "   - Publish state to Domoticz : "
           << Configuration.domoticz_publish_relay_state << endl;
  }

  Sonoff.run();
}

void loop() { Sonoff.listener(); }
