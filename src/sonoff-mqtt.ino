/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

/* Publishing state of the Relay to MQTT Broker */
void publishRelayStateMessage() {
  char  mqttString[50];
  sprintf(mqttString,"%sstate", sonoffConfig.mqtt_topic);
  if (digitalRead(RELAY)==LOW) {
    client.publish(mqttString, "OFF");
  } else {
      client.publish(mqttString, "ON");
  }
}

/* Connected to MQTT Broker */
void connectToMQTT() {
  char  mqttString[50];
  sprintf(mqttString,"Sonoff (ID: %s)",sonoffConfig.id);
  Serial << "Connecting to MQTT : " << sonoffConfig.mqtt_host << " ";
  while (!client.connected()) {
    if (client.connect(mqttString, sonoffConfig.mqtt_user, sonoffConfig.mqtt_password)) {
        Serial << endl << "Connected" << endl;
        sprintf(mqttString,"%scmd", sonoffConfig.mqtt_topic);
        client.subscribe(mqttString);
        Serial << " - Subsribed to : " << sonoffConfig.mqtt_topic << endl;
        getConfiguration();
        LEDOff();
    } else {
      delay(CONNECTION_WAIT_TIME);
      Serial << ".";
    }
  }
}

/* Callback of MQTT Broker, it listens for messages */
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char  mqttString[50];
  blinkLED();
  Serial << "Got MQTT Topic : " << topic << " ";
  if (length>=2) { // command arrived
    if((char)payload[1] == 'N') { // ON
      Serial << "ON" << endl;
      relayOn();
    } else if((char)payload[1] == 'F') { // OFF
      Serial << "turnOFF" << endl;
      relayOff();
    } else if((char)payload[2] == 'p') { // reportState
      Serial << "reportState" << endl;
      publishRelayStateMessage();
    } else if((char)payload[2] == 's') { // reset
      Serial << "reset" << endl;
      ESP.restart();
    } else if((char)payload[2] == 'n') { // configurationMode
      Serial << "configuration Mode" << endl;
      toggleMode();    
    } 
  }
}


/* Gets default configuration. Relay state and temp sensor config. It has to be implemented in the service and published over MQTT */
void getConfiguration() {  
  char  mqttString[50];
  sprintf(mqttString,"%sget", sonoffConfig.mqtt_topic);
  Serial << " - Requesting configuration : " << sonoffConfig.mqtt_topic << " " << "defaultState" <<  endl;
  client.publish(mqttString, "defaultState");
}
