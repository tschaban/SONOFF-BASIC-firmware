/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
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
#include "sonoff-switch.h"
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
SonoffSwitch    Switch;
SonoffLED       Led;
Sonoff          Sonoff;

void setup() {

  Configuration = Eeprom.getConfiguration();

  if (Configuration.debugger) {
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial << endl << "Configuration: " << endl;
    Serial << " - Version: " << Configuration.version << endl;
    Serial << " - Language: " << Configuration.language << endl;
    Serial << " - Switch mode: " << Configuration.mode << endl;
    Serial << " - Device ID: " << Configuration.id << endl;
    Serial << " - Host name: " << Configuration.device_name << endl;
    Serial << " - WiFi:" <<  endl;
    Serial << "   - SSID: " << Configuration.wifi_ssid << endl;
    Serial << "   - Password: " << Configuration.wifi_password << endl;
    Serial << " - MQTT: "  << endl;
    Serial << "   - Host: " << Configuration.mqtt_host << endl;
    Serial << "   - Port: " << Configuration.mqtt_port << endl;
    Serial << "   - User: " << Configuration.mqtt_user << endl;
    Serial << "   - Password: " << Configuration.mqtt_password << endl;
    Serial << "   - Topic: " << Configuration.mqtt_topic <<  endl;
    Serial << " - DS18B20 Sensor: " << endl;
    Serial << "   - Present: " << Configuration.ds18b20_present << endl;
    Serial << "   - Temp correctin: " << Configuration.ds18b20_correction << endl;
    Serial << "   - Temp interval: " << Configuration.ds18b20_interval << endl;
    Serial << " - External switch " << endl;
    Serial << "   - Present: " << Configuration.switch_present << endl;
    Serial << "   - GPIO: " << Configuration.switch_gpio << endl;
    Serial << "   - Sensitivness: " << Configuration.switch_sensitiveness << endl;
    Serial << " - Relay " << endl;    
    Serial << "   - State: " << Eeprom.getRelayState() << endl;
    Serial << "   - Post power restore : " << Eeprom.getRelayStateAfterPowerRestored() << endl;
    Serial << "   - Post reconnection to MQTT : " << Eeprom.getRelayStateAfterConnectionRestored() << endl;
  }
  Sonoff.run();

}

void loop() {
  Sonoff.listener();
}

