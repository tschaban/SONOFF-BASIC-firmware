/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

/* Publishing state of the Relay to MQTT Broker */
void publishRelayStateMessage() {
  char  mqttString[50];
  sprintf(mqttString,"%s/state", mqttTopic);
  if (digitalRead(RELAY)==LOW) {
    client.publish(mqttString, "OFF");
  } else {
      client.publish(mqttString, "ON");
  }
}

/* Connected to MQTT Broker */
void connectToMQTT() {
  char  mqttString[50];
  sprintf(mqttString,"Sonoff (ID: %i)",ID);
  Serial << "Connecting to MQTT : " << sonoffConfig.mqtt_host << " ";
  while (!client.connected()) {
    if (client.connect(mqttString, sonoffConfig.mqtt_user, sonoffConfig.mqtt_password)) {
        Serial << endl << "Connected" << endl;
        sprintf(mqttString,"%s/cmd", mqttTopic);
        client.subscribe(mqttString);
        Serial << " - Subsribed to : " << mqttTopic << endl;
        getConfiguration();
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
  if (length>=6) { // command arrived
    if((char)payload[5] == 'N') { // turnON
      Serial << "turnON" << endl;
      relayOn();
    } else if((char)payload[5] == 'F') { // turnOFF
      Serial << "turnOFF" << endl;
      relayOff();
    } else if((char)payload[2] == 'p') { // reportState
      Serial << "reportState" << endl;
      publishRelayStateMessage();
    } else if((char)payload[2] == 's') { // reset
      Serial << "reset" << endl;
      ESP.restart();
    } else if((char)payload[4] == 'I') { // tempInterval
        Serial << "tempInterval";
        String inString = "";
        for (int i=13;i<length;i++) {
            inString += (char)payload[i];
        }
        Serial << ":" << inString << endl;
//        setSensorReadInterval(inString.toInt());
    } else if((char)payload[4] == 'C') { // tempCorrection
        String inString = "";
        Serial << "tempCorrection";
        for (int i=15;i<length;i++) {
            inString += (char)payload[i];
        }
         Serial << ":" << inString << endl;
//        tempCorrection =  inString.toFloat();

    }
  }
}


/* Gets default configuration. Relay state and temp sensor config. It has to be implemented in the service and published over MQTT */
void getConfiguration() {  
  char  mqttString[50];
  sprintf(mqttString,"%s/get", mqttTopic);
  Serial << " - Requesting configuration : " << mqttTopic << " " << "defaultState" <<  endl;
  client.publish(mqttString, "defaultState");
}
