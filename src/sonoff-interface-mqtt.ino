/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-interface-mqtt.h"

SonoffMQTTInterface::SonoffMQTTInterface() {
}

void SonoffMQTTInterface::begin() {
  Broker.setClient(esp);
  Broker.setServer(Configuration.mqtt_host, Configuration.mqtt_port);
  Broker.setCallback(callbackMQTT);
}


void SonoffMQTTInterface::connect() {
  char  mqttString[60];
  uint8_t connection_try = 0;

  sprintf(mqttString, "Sonoff (Device name: %s)", Configuration.device_name);
  if (Configuration.debugger) Serial << endl << "INFO: Connecting to MQTT : " << Configuration.mqtt_host << ":" << Configuration.mqtt_port;

  while (!Broker.connected()) {
    if (Broker.connect(mqttString, Configuration.mqtt_user, Configuration.mqtt_password)) {
      if (Configuration.debugger) Serial << endl << "INFO: Connected";
      sprintf(mqttString, "%scmd", Configuration.mqtt_topic);
      Broker.subscribe(mqttString);
      if (Configuration.debugger) Serial << endl << "INFO: Subsribed to : " << Configuration.mqtt_topic;
    } else {
      connection_try++;
      if (Configuration.debugger) Serial << endl << "INFO: MQTT Connection attempt: " << connection_try << " from " << Configuration.number_connection_attempts;
      if (connection_try == Configuration.number_connection_attempts) {
        Sonoff.runSleepMode();
        break;
      }
      delay(Configuration.duration_between_connection_attempts*1000);      
    }
  }
  if (Configuration.debugger) Serial << endl << "INFO: MQTT connection status: " << Broker.state();
}


boolean SonoffMQTTInterface::connected() {
  return Broker.connected();
}

void SonoffMQTTInterface::publish(char* type, char* message) {
  if (Broker.state() == MQTT_CONNECTED) {
    char  mqttTopic[50];
    sprintf(mqttTopic, "%s%s", Configuration.mqtt_topic, type);
    Broker.publish(mqttTopic, message);
    if (Configuration.debugger) Serial << endl << "INFO: MQTT publising to:  " << mqttTopic << "  \\ " << message;
  } else {
    if (Configuration.debugger) Serial << endl << "WARN: MQTT not connected. State:  " << Broker.state();
  }
}

void  SonoffMQTTInterface::loop() {
  Broker.loop();
}

/* Callback of MQTT Broker, it listens for messages */
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  Led.blink();
  if (Configuration.debugger) Serial << endl << "INFO: MQTT message recieved: " << topic << " \\";
  if (length >= 1) { // command arrived
    if ((char)payload[1] == 'N') { // ON
      if (Configuration.debugger) Serial << " ON";
      Relay.on();
    } else if ((char)payload[1] == 'F') { // OFF
      if (Configuration.debugger) Serial << " OFF";
      Relay.off();
    } else if ((char)payload[2] == 'p') { // reportState
      if (Configuration.debugger) Serial << " reportState";
      Relay.publish();
    } else if ((char)payload[2] == 'b') { // reboot
      if (Configuration.debugger) Serial << " reboot";
      ESP.restart();
    } else if ((char)payload[2] == 'n') { // configurationMode
      if (Configuration.debugger) Serial << " configuration Mode";
      Sonoff.toggle();
    }
  }
}

