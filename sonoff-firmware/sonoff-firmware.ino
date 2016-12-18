#include <DNSServer.h>

#include <DallasTemperature.h>
#include <OneWire.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#include "Streaming.h"
//#include "sonoff-configuration.h"
#include "sonoff-led.h"
#include "sonoff-configuration-local.h"
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

char hostName[16] = {0};

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial << endl << "Device ID: " << ID << endl;

  sprintf(hostName, "SONOFF_%06X", ID);
  Serial << "Host name: " << hostName << endl;

  Serial.println();
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial << endl << "EEPROM" << endl;
  sonoffConfig = memory.getConfiguration();

  
  Serial << endl << "Configuration: " << endl;
  Serial << " - WiFi SSID: " << sonoffConfig.wifi_ssid << endl;
  Serial << " - WiFi Password: " << sonoffConfig.wifi_password << endl;
  Serial << " - MQTT Host: " << sonoffConfig.mqtt_host << endl;
  Serial << " - MQTT Port: " << sonoffConfig.mqtt_port << endl;
  Serial << " - MQTT User: " << sonoffConfig.mqtt_user << endl;
  Serial << " - MQTT Password: " << sonoffConfig.mqtt_password << endl;


  // configurationMode.attach(2,runConfigurationMode);

  
  Serial << endl << "Configuring MQTT" << endl;
  sprintf(mqttTopic, "%s%i", MQTT_TOPIC, ID);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callbackMQTT);

  //connectToWiFi();

  runConfigurationMode();
  

  /* 
  DS18B20.begin(); 
  setSensorReadInterval(TEMP_INTERVAL);
  buttonTimer.attach(0.1, button);  
    */
}

/* Connect to WiFI */
void connectToWiFi() {
  WiFi.hostname(hostName);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial << endl << "Connecting to WiFi: " << WIFI_SSID;
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    blinkLED(CONNECTION_WAIT_TIME / 2);
    delay(CONNECTION_WAIT_TIME / 2);
  }
  Serial << endl << " - Connected" << endl;
  Serial << " - IP: " << WiFi.localIP() << endl;
}






void loop() {
 // if (!client.connected()) {
 //   connectToMQTT();
 // }
 // client.loop();
  dnsServer.processNextRequest();
  server.handleClient();
}
