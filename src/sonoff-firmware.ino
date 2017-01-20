/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include <DallasTemperature.h>
#include <OneWire.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "Streaming.h"
#include "sonoff-configuration.h"
#include "sonoff-eeprom.h"
#include "sonoff-led.h"
#include "sonoff-relay.h"
#include "sonoff-button.h"
#include "ota.h"

DEFAULTS sonoffDefault;

SonoffEEPROM eeprom;
SonoffLED LED;
SonoffRelay Relay;
SonoffButton Button;

/* Timers */
Ticker temperatureTimer;

/* Libraries init */
WiFiClient esp;
PubSubClient client(esp);
OneWire wireProtocol(TEMP_SENSOR);
DallasTemperature DS18B20(&wireProtocol);

ESP8266WebServer server(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();

  Serial << endl << "Reading EEPROM" << endl;
  Serial << " - Version: " << eeprom.getVersion() << endl;
  Serial << " - Switch mode: " << eeprom.getMode() << endl;
  Serial << " - Device ID: " << eeprom.getID() << endl;
  Serial << " - Host name: " << eeprom.getHostName() << endl;
  Serial << " - WiFi SSID: " << eeprom.getWiFiSSID() << endl;
  Serial << " - WiFi Password: " << eeprom.getWiFiPassword() << endl;
  Serial << " - MQTT Host: " << eeprom.getMQTTHost() << endl;
  Serial << " - MQTT Port: " << eeprom.getMQTTPort() << endl;
  Serial << " - MQTT User: " << eeprom.getMQTTUser()<< endl;
  Serial << " - MQTT Password: " << eeprom.getMQTTPassword() << endl;
  Serial << " - MQTT Topic: " << eeprom.getMQTTTopic() <<  endl;
  Serial << " - DS18B20 present: " << eeprom.isDS18B20Present() << endl;
  Serial << " - Temp correctin: " << eeprom.DS18B20Correction() << endl;
  Serial << " - Temp interval: " << eeprom.DS18B20ReadInterval() << endl;

  

  Relay.setup(&LED,&eeprom,&client);
  
  if (eeprom.getWiFiSSID()[0]==(char) 0 || eeprom.getWiFiPassword()[0]==(char) 0 || eeprom.getMQTTHost()[0]==(char) 0) {
    Serial << endl << "Missing configuration. Going to configuration mode." << endl;
    eeprom.saveMode(MODE_ACCESSPOINT);
  }

   /* POST Upgrade check, version upgrade */
  if (String(sonoffDefault.version) != String(eeprom.getVersion())) {      
      Serial << endl << "SOFTWARE WAS UPGRADED from version : " << eeprom.getVersion() << " to " << sonoffDefault.version << endl;
      eeprom.saveVersion(sonoffDefault.version);     
  }
  
  if (eeprom.getMode()==MODE_CONFIGURATION) {
    Serial << endl << "Entering configuration mode over the LAN" << endl;
    runConfigurationLANMode();
  } else if (eeprom.getMode()==MODE_ACCESSPOINT) {
     Serial << endl << "Entering configuration mode with Access Point" << endl;
     runConfigurationAPMode(); 
  }else {
    Serial << endl << "Entering switch mode" << endl;
    runSwitchMode();  
  }
  
}




/* Connect to WiFI */
void connectToWiFi() {  
  WiFi.hostname(eeprom.getHostName());
  WiFi.begin(eeprom.getWiFiSSID(), eeprom.getWiFiPassword());
  Serial << endl << "Connecting to WiFi: " << eeprom.getWiFiSSID();
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    delay(CONNECTION_WAIT_TIME);
  }
  Serial << endl << " - Connected" << endl;
  Serial << " - IP: " << WiFi.localIP() << endl;
}



void loop() {
  if (eeprom.getMode()==MODE_SWITCH) {
    if (!client.connected()) {
      connectToMQTT();
    } 
    client.loop();
  } else if (eeprom.getMode()==MODE_CONFIGURATION) {      
    server.handleClient();    
  } else if (eeprom.getMode()==MODE_ACCESSPOINT) {
    dnsServer.processNextRequest();
    server.handleClient();
  } else {
    Serial << "Internal Application Error" << endl;
  }  
}
