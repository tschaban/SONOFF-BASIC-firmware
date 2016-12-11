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
  while (!client.connected()) {
    if (client.connect(mqttString, MQTT_USER, MQTT_PASSWORD)) {
        sprintf(mqttString,"%s/cmd", mqttTopic);
        client.subscribe(mqttString);
   //     getConfiguration();
    } else {
      blinkLED(CONNECTION_WAIT_TIME/2);
      delay(CONNECTION_WAIT_TIME/2);
    }
  }
}

/* Callback of MQTT Broker, it listens for messages */
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char  mqttString[50];
  blinkLED();
  if (length>=6) { // command arrived
    if((char)payload[5] == 'N') { // turnON
      relayOn();
    } else if((char)payload[5] == 'F') { // turnOFF
      relayOff();
    } else if((char)payload[2] == 'p') { // reportState
      publishRelayStateMessage();
    } else if((char)payload[2] == 's') { // reset
      ESP.restart();
    } else if((char)payload[4] == 'I') { // tempInterval
        String inString = "";
        for (int i=13;i<length;i++) {
            inString += (char)payload[i];
        }
//        setSensorReadInterval(inString.toInt());
    } else if((char)payload[4] == 'C') { // tempCorrection
        String inString = "";
        for (int i=15;i<length;i++) {
            inString += (char)payload[i];
        }
//        tempCorrection =  inString.toFloat();
    }
  }
}
