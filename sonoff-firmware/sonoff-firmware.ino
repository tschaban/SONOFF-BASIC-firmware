#include <DallasTemperature.h>
#include <OneWire.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "Streaming.h"
//#include "sonoff-configuration.h"
#include "sonoff-configuration-local.h"
#include "sonoff-relay.h"
#include "sonoff-led.h"
#include "sonoff-button.h"
#include "sonoff-ds18b20.h"



/* Variables */

/* Timers */
Ticker buttonTimer;
Ticker temperatureTimer;

/* Libraries init */
WiFiClient esp;
PubSubClient client(esp);
OneWire wireProtocol(TEMP_SENSOR);
DallasTemperature DS18B20(&wireProtocol);



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

 // sprintf(mqttTopic, "%s%i", MQTT_TOPIC, ID);

  connectToWiFi();
  DS18B20.begin();
  setSensorReadInterval(TEMP_INTERVAL);
  buttonTimer.attach(0.1, button);

}

/* Connect to WiFI */
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    blinkLED(CONNECTION_WAIT_TIME/2);
    delay(CONNECTION_WAIT_TIME/2);
  }
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();
}
