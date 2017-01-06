/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

void runSwitchMode() {
  LEDOn();
  Serial << endl << "Device mode: SWITCH" << endl;
  Serial << endl << "Configuring MQTT" << endl;
  sprintf(mqttTopic, "%s%i", sonoffConfig.mqtt_topic, ID);
  client.setServer(sonoffConfig.mqtt_host, sonoffConfig.mqtt_port);
  client.setCallback(callbackMQTT);
  connectToWiFi();
  LEDOff();
}

void runHttpUpdateMode() {
  blinkLEDInLoop(0.1);
  Serial << endl << "Device mode: HTTP UPDATE" << endl;
  WiFi.mode(WIFI_AP_STA);  
  connectToWiFi();
  Serial << " - starting HTTP Update server" << endl;
  MDNS.begin(hostName);
  httpUpdater.setup(&server);
  server.begin();
  MDNS.addService("http", "tcp", 80);
  Serial << endl << " - ready for update on http://" << WiFi.localIP() << "/update" << endl;
  memory.saveSwitchMode(0);  
}

void runConfigurationMode() {
  blinkLEDInLoop(1);
  Serial << endl << "Device mode: CONFIGURATION" << endl;
  Serial << " - launching access point" << endl;
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(hostName);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  Serial << " - launching web server" << endl;
  server.on("/", handleRoot);
  server.on("/configure", handleConfiguration);
  server.on("/update", handleUpdate);
  server.on("/reboot", handleReboot);
  server.on("/reset", handleReset);  
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial << " - ready " << endl << endl;
  
}

void toggleMode() {
  if (sonoffConfig.mode == 0) {
    memory.saveSwitchMode(1);
  } else {
    memory.saveSwitchMode(0);
  }
  Serial << "Rebooting device" << endl;
  blinkLED();
  delay(1000);
  ESP.restart();
}

void flashMode() {
  LEDOn();
  memory.saveSwitchMode(2);
  Serial << "Rebooting device in 3sek" << endl;
  delay(3000);
  ESP.restart();
}

void resetDeviceMode() {
  LEDOn();
  Serial << "- ereasing EEPROM" << endl;
  memory.erase();
  delay(1000);
  ESP.restart();
}

