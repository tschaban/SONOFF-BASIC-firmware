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

#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include "Streaming.h"
#include "sonoff-configuration.h"
#include "sonoff-led.h"
#include "sonoff-eeprom.h"

/* Variables */
SonoffEEPROM memory;
SONOFFCONFIG sonoffConfig;

/* Timers */
Ticker buttonTimer;
Ticker temperatureTimer;
Ticker configurationMode;

/* Libraries init */
WiFiClient esp;
PubSubClient client(esp);
OneWire wireProtocol(TEMP_SENSOR);
DallasTemperature DS18B20(&wireProtocol);

ESP8266WebServer server(80);
DNSServer dnsServer;
ESP8266HTTPUpdateServer httpUpdater;

char hostName[16] = {0};

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial << endl << "Device ID: " << ID << endl;

  sprintf(hostName, "SONOFF_%06X", ID);
  Serial << "Host name: " << hostName << endl;

  Serial.println();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial << endl << "EEPROM" << endl;
  sonoffConfig = memory.getConfiguration();

  Serial << endl << "Configuration: " << endl;
  Serial << " - Switch mode: " << sonoffConfig.mode << endl;
  Serial << " - WiFi SSID: " << sonoffConfig.wifi_ssid << endl;
  Serial << " - WiFi Password: " << sonoffConfig.wifi_password << endl;
  Serial << " - MQTT Host: " << sonoffConfig.mqtt_host << endl;
  Serial << " - MQTT Port: " << sonoffConfig.mqtt_port << endl;
  Serial << " - MQTT User: " << sonoffConfig.mqtt_user << endl;
  Serial << " - MQTT Password: " << sonoffConfig.mqtt_password << endl;
  Serial << " - MQTT Topic: " << sonoffConfig.mqtt_topic << ID << "/" << endl;
  Serial << " - Temp correctin: " << sonoffConfig.temp_correction << endl;
  Serial << " - Temp interval: " << sonoffConfig.temp_interval << endl;

  buttonTimer.attach(0.05, callbackButton);

  Serial << endl << "Setting relay: " << endl;
  pinMode(RELAY, OUTPUT);
  if (memory.getRelayState()==1) {
      digitalWrite(RELAY, HIGH);
  } else {
      digitalWrite(RELAY, LOW);
  }
  
  if (sonoffConfig.wifi_ssid[0]==(char) 0 || sonoffConfig.wifi_password[0]==(char) 0 || sonoffConfig.mqtt_host[0]==(char) 0) {
    Serial << endl << "Missing configuration. Going to configuration mode." << endl;
    memory.saveSwitchMode(1);
    sonoffConfig.mode=1;
  }
  
  if (sonoffConfig.mode==1) {
    Serial << endl << "Entering configuration mode" << endl;
    runConfigurationMode();
  } else if (sonoffConfig.mode==2) {
     Serial << endl << "Entering update mode" << endl;
     runHttpUpdateMode(); 
  }else {
    Serial << endl << "Entering switch mode" << endl;
    runSwitchMode();  
  }

  /* 
  DS18B20.begin(); 
  setSensorReadInterval(TEMP_INTERVAL);
  buttonTimer.attach(0.1, button);  
    */
   
}




/* Connect to WiFI */
void connectToWiFi() {
  WiFi.hostname(hostName);
  WiFi.begin(sonoffConfig.wifi_ssid, sonoffConfig.wifi_password);
  Serial << endl << "Connecting to WiFi: " << sonoffConfig.wifi_ssid;
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    delay(CONNECTION_WAIT_TIME);
  }
  Serial << endl << " - Connected" << endl;
  Serial << " - IP: " << WiFi.localIP() << endl;
}



void loop() {
  if (sonoffConfig.mode==0) {
    if (!client.connected()) {
      connectToMQTT();
     }
    client.loop();
  } else if (sonoffConfig.mode==1) {  
    dnsServer.processNextRequest();
    server.handleClient();
  } else if (sonoffConfig.mode==2) {
    server.handleClient();
  } else {
    Serial << "Internal Application Error" << endl;
  }  
}
