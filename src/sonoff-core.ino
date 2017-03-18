/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
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
    if (Configuration.debugger) Serial << "Internal Application error" << endl;
  }
}

void Sonoff::reset() {
  if (Configuration.debugger) Serial << "- ereasing EEPROM" << endl;
  Eeprom.erase();
  ESP.restart();
}

void Sonoff::toggle() {
  if (Configuration.mode == MODE_SWITCH) {
    Eeprom.saveMode(MODE_CONFIGURATION);
  } else {
    Eeprom.saveMode(MODE_SWITCH);
  }
  if (Configuration.debugger) Serial << "Rebooting device" << endl;
  Led.blink();
  delay(10);
  ESP.restart();
}

void Sonoff::connectWiFi() {
  WiFi.hostname(Configuration.device_name);
  WiFi.begin(Configuration.wifi_ssid, Configuration.wifi_password);
  if (Configuration.debugger) Serial << endl << "Connecting to WiFi: " << Configuration.wifi_ssid << endl;
  while (WiFi.status() != WL_CONNECTED) {
    if (Configuration.debugger) Serial << ".";
    delay(CONNECTION_WAIT_TIME);
  }
  if (Configuration.debugger) Serial << endl << " - Connected" << endl;
  if (Configuration.debugger) Serial << " - IP: " << WiFi.localIP() << endl;
}

void Sonoff::connectMQTT() {
  char  mqttString[50];

  Mqtt.setServer(Configuration.mqtt_host, Configuration.mqtt_port);
  Mqtt.setCallback(callbackMQTT);

  sprintf(mqttString, "Sonoff (ID: %s)", Configuration.id);
  if (Configuration.debugger) Serial << "Connecting to MQTT : " << Configuration.mqtt_host << ":" << Configuration.mqtt_port << endl;
  if (Configuration.debugger) Serial << " - user : " << Configuration.mqtt_user << endl;
  if (Configuration.debugger) Serial << " - password : " << Configuration.mqtt_password << endl;

  while (!Mqtt.connected()) {
    if (Mqtt.connect(mqttString, Configuration.mqtt_user, Configuration.mqtt_password)) {
      if (Configuration.debugger) Serial << endl << "Connected" << endl;
      sprintf(mqttString, "%scmd", Configuration.mqtt_topic);
      Mqtt.subscribe(mqttString);
      if (Configuration.debugger) Serial << " - Subsribed to : " << Configuration.mqtt_topic << endl;

      /* Post connection relay set up */
      if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_ON && Relay.get() == RELAY_OFF) {
        Relay.on();
      } else if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_OFF && Relay.get() == RELAY_ON) {
        Relay.off();
      } else if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_LAST_KNOWN) {
        if (Eeprom.getRelayState() == 0 && Relay.get() == RELAY_ON) {
          Relay.on();
        } else if (Eeprom.getRelayState() == 1 && Relay.get() == RELAY_OFF) {
          Relay.off();
        }
      } else  {
        getRelayServerValue();
      }

      Led.off();
    } else {
      delay(CONNECTION_WAIT_TIME);
      if (Configuration.debugger) Serial << " - mqtt connection status: " << Mqtt.state();
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
    if (Configuration.debugger) Serial << "Internal Application Error" << endl;
  }
}

void Sonoff::setDS18B20Interval( unsigned int interval) {
  temperatureTimer.detach();
  temperatureTimer.attach(interval, callbackDS18B20);
}

void Sonoff::publishTemperature(float temperature) {
  char  temperatureString[6];
  char  mqttString[50];

  dtostrf(temperature, 2, 2, temperatureString);
  if (previousTemperature != temperature) {
    if (Configuration.debugger) Serial << " - publishing: " << temperatureString << endl;
    sprintf(mqttString, "%stemperature", Configuration.mqtt_topic);
    Mqtt.publish(mqttString, temperatureString);
    previousTemperature = temperature;
  }
}

void Sonoff::getRelayServerValue() {
  char  mqttString[50];
  sprintf(mqttString, "%sget", Configuration.mqtt_topic);
  if (Configuration.debugger) Serial << endl << " Requesting default relay value";
  Mqtt.publish(mqttString, "defaultState");
  if (Configuration.debugger) Serial << ", completed" << endl;
}

/* Private methods */

void Sonoff::runSwitch() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "Device mode: SWITCH" << endl;
  if (Configuration.debugger) Serial << endl << "Configuring MQTT" << endl;
  WiFi.mode(WIFI_STA);
  connectWiFi();
  WiFi.mode(WIFI_STA);
  if (Configuration.ds18b20_present) {
    if (Configuration.debugger) Serial << endl << "Starting DS18B20" << endl;
    setDS18B20Interval(Configuration.ds18b20_interval);
  } else {
    if (Configuration.debugger) Serial << endl << "DS18B20 not present" << endl;
  }
  if (Configuration.switch_present) {
    if (Configuration.debugger) Serial << endl << "Initiating external switch" << endl;
    Switch.init(Configuration.switch_gpio);
  } else {
      if (Configuration.debugger) Serial << endl << "External switch present" << endl;
  }  
}

void Sonoff::runConfigurationLAN() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "Device mode: LAN Configuration" << endl;  
  WiFi.mode(WIFI_STA);
  connectWiFi();
  WiFi.mode(WIFI_STA);
  startHttpServer();
  if (Configuration.debugger) Serial << endl << " - Ready for configuration. Open http://" << WiFi.localIP() << endl << endl;
  Led.startBlinking(0.1);
}

void Sonoff::runConfigurationAP() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "Device mode: Access Point Configuration" << endl;
  if (Configuration.debugger) Serial << " - launching access point" << endl;
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(Configuration.device_name);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  startHttpServer();
  if (Configuration.debugger) Serial << " - After conecting to WiFi: " << Configuration.device_name << " open: http://192.168.5.1/  " << endl << endl;
  Led.startBlinking(0.1);
}

boolean Sonoff::isConfigured() {
  if (Configuration.wifi_ssid[0] == (char) 0 || Configuration.wifi_password[0] == (char) 0 || Configuration.mqtt_host[0] == (char) 0 || Configuration.mqtt_host[0] == (char) 0 ) {
    if (Configuration.debugger) Serial << endl << "Missing configuration. Going to configuration mode." << endl;
    Eeprom.saveMode(MODE_ACCESSPOINT);
    Configuration = Eeprom.getConfiguration();
  }
}

void Sonoff::postUpgradeCheck() {
  if (String(sonoffDefault.version) != String(Configuration.version)) {
    if (Configuration.debugger) Serial << endl << "SOFTWARE WAS UPGRADED from version : " << Configuration.version << " to " << sonoffDefault.version << endl;
    Eeprom.saveVersion(sonoffDefault.version);


    if (String(Configuration.version) == "0.3.2") {
      Eeprom.saveRelayStateAfterPowerRestored(sonoffDefault.relay_state_after_power_restored);
    }

    if (String(Configuration.version) == "0.3.2" ||
        String(Configuration.version) == "0.4.0") {
      Eeprom.saveLanguage(sonoffDefault.language);
    }

    if (String(Configuration.version) == "0.3.2" ||
        String(Configuration.version) == "0.4.0" ||
        String(Configuration.version) == "0.5.0") {

      char _id[6] = {0};
      char _device_name[32] = {0};

      sprintf(_id, "%06X", ESP.getChipId());
      sprintf(_device_name, "SONOFF_%s", _id);
      Eeprom.saveDeviceName(_device_name);

      /* After Connecton is restored parameter is set to saved After Power is restored value */
      Eeprom.saveRelayStateAfterConnectionRestored(Eeprom.getRelayStateAfterPowerRestored());

      /* Value "Server value" [4] is not longer needed for parameter  After Power Restored.
          If it was set like that  by a user it's upgraded to value 2 which is relay state OFF
      */
      if (Eeprom.getRelayStateAfterPowerRestored() == 4) {
        Eeprom.saveRelayStateAfterPowerRestored(2);
      }
    }

    if (String(Configuration.version) == "0.3.2" ||
        String(Configuration.version) == "0.4.0" ||
        String(Configuration.version) == "0.5.0" ||
        String(Configuration.version) == "0.6.1") {
      Eeprom.saveSwitchPresent(sonoffDefault.switch_present);
      Eeprom.saveSwitchGPIO(sonoffDefault.switch_gpio);
      Eeprom.saveSwitchSensitiveness(sonoffDefault.switch_sensitiveness);
      Eeprom.saveDebuggable(0);
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
  Led.blink();
  if (Configuration.debugger) Serial << "Got MQTT Topic : " << topic << ", length=" << length;
  if (length >= 1) { // command arrived
    if ((char)payload[1] == 'N') { // ON
      if (Configuration.debugger) Serial << " ON" << endl;
      Relay.on();
    } else if ((char)payload[1] == 'F') { // OFF
      if (Configuration.debugger) Serial << " OFF" << endl;
      Relay.off();
    } else if ((char)payload[2] == 'p') { // reportState
      if (Configuration.debugger) Serial << " reportState" << endl;
      Relay.publish();
    } else if ((char)payload[2] == 's') { // reset
      if (Configuration.debugger) Serial << " reset" << endl;
      ESP.restart();
    } else if ((char)payload[2] == 'n') { // configurationMode
      if (Configuration.debugger) Serial << " configuration Mode" << endl;
      Sonoff.toggle();
    }
  }
  if (Configuration.debugger) Serial << endl;
}


