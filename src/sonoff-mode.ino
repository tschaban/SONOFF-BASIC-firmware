/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

void runSwitchMode() {
  LED.On();
  Serial << endl << "Device mode: SWITCH" << endl;
  Serial << endl << "Configuring MQTT" << endl;
  client.setServer(eeprom.getMQTTHost(), eeprom.getMQTTPort());
  client.setCallback(callbackMQTT);
  connectToWiFi();

  if (eeprom.isDS18B20Present()) {
    Serial << endl << "Starting DS18B20" << endl;
    DS18B20.begin(); 
    setSensorReadInterval(eeprom.DS18B20ReadInterval());
  } else {
    Serial << endl << "DS18B20 not present" << endl; 
  }
}

void runConfigurationLANMode() {
  LED.On();
  Serial << endl << "Device mode: LAN Configuration" << endl;
  WiFi.mode(WIFI_STA);
  connectToWiFi();
  startHttpServer();
  Serial << endl << " - Ready for configuration. Open http://" << WiFi.localIP() << endl << endl;
  LED.startBlinking(0.1);
}

void runConfigurationAPMode() {
  LED.On();
  Serial << endl << "Device mode: Access Point Configuration" << endl;
  Serial << " - launching access point" << endl;
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(eeprom.getHostName());
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  startHttpServer();
  Serial << " - After conecting to WiFi: " << eeprom.getHostName() << " open: http://192.168.5.1/  " << endl << endl;
  LED.startBlinking(0.1);  
}

void toggleMode() {
  if (sonoffConfig.mode == 0) {
    eeprom.saveSwitchMode(1);
  } else {
    eeprom.saveSwitchMode(0);
  }
  Serial << "Rebooting device" << endl;
  LED.blink();
  delay(1000);
  ESP.restart();
}

void configuratonAPMode() {
  LED.On();
  eeprom.saveSwitchMode(2);
  Serial << "Rebooting device to Access Point" << endl;
  delay(1000);
  ESP.restart();
}

void resetDeviceMode() {
  LED.On();
  Serial << "- ereasing EEPROM" << endl;
  eeprom.erase();
  delay(1000);
  ESP.restart();
}

