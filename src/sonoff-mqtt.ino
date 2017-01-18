/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/


/* Connected to MQTT Broker */
void connectToMQTT() {
  char  mqttString[50];
  sprintf(mqttString,"Sonoff (ID: %s)",eeprom.getID());
  Serial << "Connecting to MQTT : " << eeprom.getMQTTHost() << " ";
  while (!client.connected()) {
    if (client.connect(mqttString, eeprom.getMQTTUser(), eeprom.getMQTTPassword())) {
        Serial << endl << "Connected" << endl;
        sprintf(mqttString,"%scmd", eeprom.getMQTTTopic());
        client.subscribe(mqttString);
        Serial << " - Subsribed to : " << eeprom.getMQTTTopic() << endl;
        LED.Off();
    } else {
      delay(CONNECTION_WAIT_TIME);
      Serial << ".";
    }
  }
}

/* Callback of MQTT Broker, it listens for messages */
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char  mqttString[50];
  LED.blink();
  Serial << "Got MQTT Topic : " << topic << ", length=" << length;
  if (length>=1) { // command arrived
    if((char)payload[1] == 'N') { // ON
      Serial << "ON" << endl;
      Relay.on();
    } else if((char)payload[1] == 'F') { // OFF
      Serial << "turnOFF" << endl;
      Relay.off();
    } else if((char)payload[2] == 'p') { // reportState
      Serial << "reportState" << endl;
      Relay.publish();
    } else if((char)payload[2] == 's') { // reset
      Serial << "reset" << endl;
      ESP.restart();
    } else if((char)payload[2] == 'n') { // configurationMode
      Serial << "configuration Mode" << endl;
      toggleMode();    
    } 
  }
  Serial << endl;
}

