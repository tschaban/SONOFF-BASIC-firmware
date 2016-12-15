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

/* Timers */
Ticker buttonTimer;
Ticker temperatureTimer;

/* Libraries init */
WiFiClient esp;
PubSubClient client(esp);
OneWire wireProtocol(TEMP_SENSOR);
DallasTemperature DS18B20(&wireProtocol);

ESP8266WebServer server(80);

SonoffEEPROM memory;

void setup() {
  Serial.begin(115200);  
  delay(10);
 

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callbackMQTT);

  Serial.println();
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial << " Device ID: " << ID << endl;

  sprintf(mqttTopic, "%s%i", MQTT_TOPIC, ID);

  memory.read();

  connectToWiFi();
  DS18B20.begin();
  setSensorReadInterval(TEMP_INTERVAL);
  buttonTimer.attach(0.1, button);

  server.on("/", handleRoot);
  server.on("/configure", handleConfiguration);
  server.on("/update", handleUpdate);


  server.onNotFound(handleNotFound);
  server.begin();

}

/* Connect to WiFI */
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial << "Connecting to WiFi: " << WIFI_SSID;
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    blinkLED(CONNECTION_WAIT_TIME/2);
    delay(CONNECTION_WAIT_TIME/2);
  }
  Serial << endl << "Connected" << endl;
  Serial << "IP: " << WiFi.localIP() << endl;
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();
  server.handleClient();
}
