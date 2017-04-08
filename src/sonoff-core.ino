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
  
  SonoffFirmware Firmware;
  if (Firmware.upgraded()) {
    Firmware.update();
  }
  
  Firmware = {};

  if (Configuration.mode == MODE_SWITCH) {
    runSwitch();
  } else if (Configuration.mode == MODE_CONFIGURATION) {
    runConfigurationLAN();
  } else { /* Configuration: Access point */
    runConfigurationAP();
  }
}

/* It removed configuration */
void Sonoff::reset() {
  Eeprom.erase();
  ESP.restart();
}

/* Toggle Sonoff between switch and configuration mode */
void Sonoff::toggle() {
  if (Configuration.mode == MODE_SWITCH) {
    Eeprom.saveMode(MODE_CONFIGURATION);
  } else {
    Eeprom.saveMode(MODE_SWITCH);
  }
  if (Configuration.debugger) Serial <<  endl << "WARN: Rebooting device";
  Led.blink();
  delay(10);
  ESP.restart();
}

/* Connecting to WiFi */
void Sonoff::connectWiFi() {
  WiFi.hostname(Configuration.device_name);
  WiFi.begin(Configuration.wifi_ssid, Configuration.wifi_password);
  if (Configuration.debugger) Serial << endl << "INFO: Connecting to WiFi: " << Configuration.wifi_ssid;
  Button.stop(); // Turning off button while connecting to WiFi
  if (Configuration.debugger) Serial << endl;
  while (WiFi.status() != WL_CONNECTED) {
    if (Configuration.debugger) Serial << ".";
    delay(CONNECTION_WAIT_TIME);
  }
  Button.start();  // Turning on button while connecting to WiFi
  if (Configuration.debugger) Serial << endl << "INFO: Connected";
  if (Configuration.debugger) Serial << endl << "INFO: IP: " << WiFi.localIP();
}


void Sonoff::listener() {
  if (Configuration.mode == MODE_SWITCH) {
    if (Configuration.interface == INTERFACE_MQTT) {
      if (WiFi.status() == WL_CONNECTED) {
        if (!Mqtt.connected()) {
          Led.on();
          Button.stop(); // If not connected to Mqtt, turn off button
          Mqtt.connect();
          Button.start(); // If not connected to Mqtt, turn off button
          setRelayAfterConnectingToMQTT();
          Led.off();
        }
        Mqtt.loop();
      }
    } else if (Configuration.interface == INTERFACE_HTTP) {
      server.handleClient();
    }
  } else if (Configuration.mode == MODE_CONFIGURATION) {
    server.handleClient();
  } else if (Configuration.mode == MODE_ACCESSPOINT) {
    dnsServer.processNextRequest();
    server.handleClient();
  } else {
    if (Configuration.debugger) Serial << endl << "ERROR: Internal Application Error";
  }
}

void Sonoff::setDS18B20Interval( unsigned int interval) {
  temperatureTimer.detach();
  temperatureTimer.attach(interval, callbackDS18B20);
}

void Sonoff::publishTemperature(float temperature) {
  if (previousTemperature != temperature) {
    char  temperatureString[6];
    dtostrf(temperature, 2, 2, temperatureString);
    Mqtt.publish((char*)"temperature", temperatureString);
    previousTemperature = temperature;
  }

}


/* Private methods */



/* Method: launch into swich mode */
void Sonoff::runSwitch() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "INFO: Device mode: SWITCH";
  if (Configuration.interface != INTERFACE_NONE) { /* If not standalone mode connect to WiFi and run DS18B20 sensor if configured */
    WiFi.mode(WIFI_STA); /* @TODO: does it make sense? */
    connectWiFi();
    if (Configuration.interface == INTERFACE_HTTP) {
      HttpInterface.begin();
      Led.off();
    } else if (Configuration.interface == INTERFACE_MQTT) {
      Mqtt.begin();
    }
    WiFi.mode(WIFI_STA);
    if (Configuration.ds18b20_present) {
      if (Configuration.debugger) Serial << endl << "INFO: Starting DS18B20";
      setDS18B20Interval(Configuration.ds18b20_interval);
    } else {

      if (Configuration.debugger) Serial << endl << "INFO: DS18B20 not present";
    }
  }

  if (Configuration.switch_present) { /* Run external switch if configured */
    if (Configuration.debugger) Serial << endl << "INFO: Initiating external switch";
    Switch.init(Configuration.switch_gpio);
  } else {
    if (Configuration.debugger) Serial << endl << "INFO: External switch not present";
  }
}

void Sonoff::runConfigurationLAN() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "INFO: Device mode: LAN Configuration";
  WiFi.mode(WIFI_STA);
  connectWiFi();
  startHttpServer();
  WiFi.mode(WIFI_STA);
  if (Configuration.debugger) Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
  Led.startBlinking(0.1);
}

void Sonoff::runConfigurationAP() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "INFO: Device mode: Access Point Configuration";
  if (Configuration.debugger) Serial << endl << "INFO: launching access point";
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(Configuration.device_name);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  startHttpServer();
  if (Configuration.debugger) Serial << endl << "INFO: Open: http://192.168.5.1/  ";
  Led.startBlinking(0.1);
}


void Sonoff::setRelayAfterConnectingToMQTT() {
  if (Configuration.debugger) Serial << endl << "INFO: Setting relay post MQTT connection";
  if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_ON) {
    Relay.on();
  } else if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_OFF) {
    Relay.off();
  } else if (Eeprom.getRelayStateAfterConnectionRestored() == DEFAULT_RELAY_LAST_KNOWN) {
    if (Eeprom.getRelayState() == 0) {
      Relay.on();
    } else if (Eeprom.getRelayState() == 1) {
      Relay.off();
    }
  } else  {
    Mqtt.publish((char*)"state", (char*)"defaultState");
  }
}

boolean Sonoff::isConfigured() {
  if (Configuration.wifi_ssid[0] == (char) 0 || Configuration.wifi_password[0] == (char) 0 || Configuration.mqtt_host[0] == (char) 0 || Configuration.mqtt_host[0] == (char) 0 ) {
    if (Configuration.debugger) Serial << endl << "WARN: Missing configuration. Going to configuration mode.";
    Eeprom.saveMode(MODE_ACCESSPOINT);
    Configuration = Eeprom.getConfiguration();
  }
}



void callbackDS18B20() {
  SonoffDS18B20 Temperature;
  float temperature = Temperature.get();

  Sonoff.publishTemperature(temperature);
}


