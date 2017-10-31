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

  /* Code checing if firmware has been upgraded */
  SonoffFirmware Firmware;
  if (Firmware.upgraded()) {
    Firmware.update();
  }
  Firmware = {};

  /* Launch one of the 3 switch modes */
  if (Configuration.mode == MODE_SWITCH) {
    runSwitch();
  } else if (Configuration.mode == MODE_CONFIGURATION) {
    runConfigurationLAN();
  } else { /* Configuration: Access point */
    runConfigurationAP();
  }
}

/* It resets configuration */
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
  uint8_t connection_try = 0;

  WiFi.hostname(Configuration.device_name);
  WiFi.begin(Configuration.wifi_ssid, Configuration.wifi_password);
  if (Configuration.debugger) Serial << endl << "INFO: WiFi connection status: " << WiFi.status();
  if (Configuration.debugger) Serial << endl << "INFO: Connecting to WiFi: " << Configuration.wifi_ssid;

  Button.stop(); // Turning off button while connecting to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    connection_try++;
    if (Configuration.debugger) Serial << endl << "INFO: WiFi connection attempt: " << connection_try << " from " << Configuration.number_connection_attempts;
    delay(Configuration.duration_between_connection_attempts * 1000);
    if (connection_try == Configuration.number_connection_attempts) {
      runSleepMode();
      break;
    }
  }
  Button.start();  // Turning on button while connecting to WiFi
  if (Configuration.debugger) {
    if (!Configuration.sleep_mode) {
      Serial << endl << "INFO: Connected";
      Serial << endl << "INFO: IP: " << WiFi.localIP();
    } else {
      Serial << endl << "WARN: Not Connected";
    }
  }
}

/* Main loop method */
void Sonoff::listener() {
  if (!Configuration.sleep_mode) {
    if (Configuration.mode == MODE_SWITCH) {
      if (Configuration.interface == INTERFACE_MQTT) {
        if (WiFi.status() == WL_CONNECTED) {
          if (!MqttInterface.connected()) {
            Led.on();
            Button.stop(); // If not connected to Mqtt, turn off button
            MqttInterface.connect();
            Button.start(); // If not connected to Mqtt, turn off button
            if (MqttInterface.connected()) setRelayAfterConnectingToMQTT();
            Led.off();
          }
          MqttInterface.loop();
        } else {
          runSleepMode(); // Not connected to WiFi
        }
      } else if (Configuration.interface == INTERFACE_HTTP) {
        if (WiFi.status() == WL_CONNECTED) {
          server.handleClient();
          if (Configuration.domoticz_publish_relay_state) {
            DomoticzInterface.pushRequest(); // If calls URL to Domotoczu with eg status of switch
          }
        } else {
          runSleepMode(); // Not connected to WiFi
        }
      }
    } else if (Configuration.mode == MODE_CONFIGURATION) {
      if (WiFi.status() == WL_CONNECTED) {
        server.handleClient();
      } else {
        runSleepMode(); // Not connected to WiFi
      }
    } else if (Configuration.mode == MODE_ACCESSPOINT) {
      dnsServer.processNextRequest();
      server.handleClient();
    } else {
      if (Configuration.debugger) Serial << endl << "ERROR: Internal Application Error";
    }
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
    MqttInterface.publish((char*)"temperature", temperatureString);
    previousTemperature = temperature;
  }
}

void Sonoff::stopSleepMode() {
  sleepModeTimer.detach();
  if (Configuration.debugger) Serial <<  endl << "INFO: Sleep mode turned off";
}

/* Private methods */



/* Method: launch into swich mode */
void Sonoff::runSleepMode() {
  if (Configuration.debugger) Serial << endl << "WARN: Going to sleep mode";
  Configuration.sleep_mode = true;
  Led.startBlinking(1);
  sleepModeTimer.attach(Configuration.duration_between_next_connection_attempts_series * 60, callbackSleepMode);
}


void Sonoff::runSwitch() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "INFO: Device mode: SWITCH";
  if (Configuration.interface != INTERFACE_NONE) { /* If not standalone mode connect to WiFi and run DS18B20 sensor if configured */
    WiFi.mode(WIFI_STA); /* @TODO: does it make sense? */
    connectWiFi();
    if (Configuration.interface == INTERFACE_HTTP) {
      HttpInterface.begin();
      if (Configuration.domoticz_publish_relay_state) {
        DomoticzInterface.begin();
      }
    } else if (Configuration.interface == INTERFACE_MQTT) {
      MqttInterface.begin();
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
    Switch.init(Configuration.switch_gpio, Configuration.switch_type);
  } else {
    if (Configuration.debugger) Serial << endl << "INFO: External switch not present";
  }

  Led.off();
}

void Sonoff::runConfigurationLAN() {
  Led.on();
  if (Configuration.debugger) Serial << endl << "INFO: Device mode: LAN Configuration";
  WiFi.mode(WIFI_STA);
  connectWiFi();
  if (!Configuration.sleep_mode) {
    startHttpServer();
    Led.startBlinking(0.1);
    if (Configuration.debugger) Serial << endl << "INFO: Ready for configuration. Open http://" << WiFi.localIP();
  } else {
    if (Configuration.debugger) Serial << endl << "WARN: Not connected to WiFi - configuration is not possible. Try Access Point mode - press button for 12s";
    toggle();
  }
  WiFi.mode(WIFI_STA);
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
    MqttInterface.publish((char*)"state", (char*)"defaultState");
  }
}

boolean Sonoff::isConfigured() {
  if (Configuration.wifi_ssid[0] == (char) 0 || Configuration.wifi_password[0] == (char) 0) {
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

void callbackSleepMode() {
  if (Configuration.debugger) Serial << endl << "INFO: Checking if connected to WiFi.";
  if (WiFi.status() == WL_CONNECTED) {
    Configuration.sleep_mode = false;
    Led.stopBlinking();
    Sonoff.stopSleepMode();
  }
}
