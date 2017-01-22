/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-core.h"

Sonoff::Sonoff() {
}

void Sonoff::run() {

  isConfigured();
  postUpgradeCheck();

  if (Configuration.mode == MODE_SWITCH) {
    runSwitch();
  } else if (Configuration.mode == MODE_CONFIGURATION) {
    runConfigurationLAN();
  } else if (Configuration.mode == MODE_ACCESSPOINT) {
    runConfigurationAP();
  } else {
    Serial << "Internal Application error" << endl;
  }
}

void Sonoff::reset() {
  Serial << "- ereasing EEPROM" << endl;
  Eeprom.erase();
  ESP.restart();
}

void Sonoff::toggle() {
  if (Configuration.mode == MODE_SWITCH) {
    Eeprom.saveMode(MODE_CONFIGURATION);
  } else {
    Eeprom.saveMode(MODE_SWITCH);
  }
  Serial << "Rebooting device" << endl;
  Led.blink();
  delay(10);
  ESP.restart();
}

void Sonoff::connectWiFi() {
  WiFi.hostname(Configuration.host_name);
  WiFi.begin(Configuration.wifi_ssid, Configuration.wifi_password);
  Serial << endl << "Connecting to WiFi: " << Configuration.wifi_ssid << endl;
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    delay(CONNECTION_WAIT_TIME);
  }
  Serial << endl << " - Connected" << endl;
  Serial << " - IP: " << WiFi.localIP() << endl;
}

void Sonoff::connectMQTT() {
  char  mqttString[50];
  
  Mqtt.setServer(Configuration.mqtt_host, Configuration.mqtt_port);
  Mqtt.setCallback(callbackMQTT);
  
  sprintf(mqttString, "Sonoff (ID: %s)", Configuration.id);
  Serial << "Connecting to MQTT : " << Configuration.mqtt_host << ":" << Configuration.mqtt_port << endl;
  Serial << " - user : " << Configuration.mqtt_user << endl;
  Serial << " - password : " << Configuration.mqtt_password << endl;
 
  while (!Mqtt.connected()) {
    if (Mqtt.connect(mqttString, Configuration.mqtt_user, Configuration.mqtt_password)) {
      Serial << endl << "Connected" << endl;
      sprintf(mqttString, "%scmd", Configuration.mqtt_topic);
      Mqtt.subscribe(mqttString);
      Serial << " - Subsribed to : " << Configuration.mqtt_topic << endl;

      /* Updating server with relay state or reading the value from the server */
      if (Eeprom.getRelayStartState() == DEFAULT_RELAY_SERVER) {
        getRelayServerValue();
      } else {
        Relay.publish();
      }

      Led.off();
    } else {
      delay(CONNECTION_WAIT_TIME);
      Serial << " - mqtt connection status: " << Mqtt.state();
    }
  }
}

void Sonoff::listener() {
  if (Configuration.mode == MODE_SWITCH) {
    if (!Mqtt.connected()) {
      connectMQTT();
    }
    Mqtt.loop();
  } else if (Configuration.mode == MODE_CONFIGURATION) {
    server.handleClient();
  } else if (Configuration.mode == MODE_ACCESSPOINT) {
    dnsServer.processNextRequest();
    server.handleClient();
  } else {
    Serial << "Internal Application Error" << endl;
  }
}

void Sonoff::setDS18B20Interval( int interval) {
  temperatureTimer.detach();
  temperatureTimer.attach(interval, callbackDS18B20);
}

void Sonoff::publishTemperature(float temperature) {
  char  temperatureString[6];
  char  mqttString[50];

  dtostrf(temperature, 2, 2, temperatureString);
  if (previousTemperature != temperature) {
    Serial << " - publishing: " << temperatureString << endl;
    sprintf(mqttString, "%stemperature", Configuration.mqtt_topic);
    Mqtt.publish(mqttString, temperatureString);
    previousTemperature = temperature;
  }
}

void Sonoff::getRelayServerValue() {
  char  mqttString[50];
  sprintf(mqttString, "%sget", Configuration.mqtt_topic);
  Serial << endl << " Requesting default relay value";
  Mqtt.publish(mqttString, "defaultState");
  Serial << ", completed" << endl;
}

/* Private methods */

void Sonoff::runSwitch() {
  Led.on();
  Serial << endl << "Device mode: SWITCH" << endl;
  Serial << endl << "Configuring MQTT" << endl;
  connectWiFi();
  if (Configuration.ds18b20_present) {
    Serial << endl << "Starting DS18B20" << endl;
    setDS18B20Interval(Configuration.ds18b20_interval);
  } else {
    Serial << endl << "DS18B20 not present" << endl;
  }

}

void Sonoff::runConfigurationLAN() {
  Led.on();
  Serial << endl << "Device mode: LAN Configuration" << endl;
  WiFi.mode(WIFI_STA);
  connectWiFi();
  startHttpServer();
  Serial << endl << " - Ready for configuration. Open http://" << WiFi.localIP() << endl << endl;
  Led.startBlinking(0.1);
}

void Sonoff::runConfigurationAP() {
  Led.on();
  Serial << endl << "Device mode: Access Point Configuration" << endl;
  Serial << " - launching access point" << endl;
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(Configuration.host_name);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  startHttpServer();
  Serial << " - After conecting to WiFi: " << Configuration.host_name << " open: http://192.168.5.1/  " << endl << endl;
  Led.startBlinking(0.1);
}

boolean Sonoff::isConfigured() {
  if (Configuration.wifi_ssid[0] == (char) 0 || Configuration.wifi_password[0] == (char) 0 || Configuration.mqtt_host[0] == (char) 0) {
    Serial << endl << "Missing configuration. Going to configuration mode." << endl;
    Eeprom.saveMode(MODE_ACCESSPOINT);
    Configuration = Eeprom.getConfiguration();
  }
}

void Sonoff::postUpgradeCheck() {
  if (String(sonoffDefault.version) != String(Configuration.version)) {
    Serial << endl << "SOFTWARE WAS UPGRADED from version : " << Configuration.version << " to " << sonoffDefault.version << endl;
    Eeprom.saveVersion(sonoffDefault.version);

    if (String(sonoffDefault.version) == "0.4.0") {
      Eeprom.saveRelayDefaultState(sonoffDefault.relay_post_crash);
    }
    Configuration = Eeprom.getConfiguration();
  }
}

void callbackDS18B20() {
  SonoffDS18B20 Temperature;
  float temperature = Temperature.get();

  Sonoff.publishTemperature(temperature);
}

/* Callback of MQTT Broker, it listens for messages */
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  char  mqttString[50];
  Led.blink();
  Serial << "Got MQTT Topic : " << topic << ", length=" << length;
  if (length >= 1) { // command arrived
    if ((char)payload[1] == 'N') { // ON
      Serial << " ON" << endl;
      Relay.on();
    } else if ((char)payload[1] == 'F') { // OFF
      Serial << " OFF" << endl;
      Relay.off();
    } else if ((char)payload[2] == 'p') { // reportState
      Serial << " reportState" << endl;
      Relay.publish();
    } else if ((char)payload[2] == 's') { // reset
      Serial << " reset" << endl;
      ESP.restart();
    } else if ((char)payload[2] == 'n') { // configurationMode
      Serial << " configuration Mode" << endl;
      Sonoff.toggle();
    }
  }
  Serial << endl;
}

