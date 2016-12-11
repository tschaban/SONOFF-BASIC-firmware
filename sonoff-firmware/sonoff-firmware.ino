#include <DallasTemperature.h>
#include <OneWire.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "Streaming.h"
#include "sonoff-configuration.h"
#include "sonoff-relay.h"
#include "sonoff-led.h"
#include "sonoff-button.h"


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
  // put your setup code here, to run once:

}

void loop() {
  delay(5000);
  blinkLED();
}
