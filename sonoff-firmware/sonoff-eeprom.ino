/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-eeprom.h"

SonoffEEPROM::SonoffEEPROM() {
  EEPROM.begin(EEPROM_size);
}


SONOFFCONFIG SonoffEEPROM::getConfiguration() {
  Serial << "Reading configuration from EEPROM" << endl;
  SONOFFCONFIG _temp;

  _temp.version = 0.1;
  _temp.mode = getSwitchMode().toInt();

  Serial << "Device mode in EEPROM=" << _temp.mode << endl;
  
  // It switch_mode not set set it to configuraion mode = 1
  if (!(_temp.mode == 0 || _temp.mode == 1 || _temp.mode == 2)) {
    erase();
    _temp.mode = 1;
  }

  getWiFiSSID().toCharArray(_temp.wifi_ssid, sizeof(_temp.wifi_ssid));
  getWiFiPassword().toCharArray(_temp.wifi_password, sizeof(_temp.wifi_password));
  getMQTTHost().toCharArray(_temp.mqtt_host, sizeof(_temp.mqtt_host));  
  getMQTTUser().toCharArray(_temp.mqtt_user, sizeof(_temp.mqtt_user));
  getMQTTPassword().toCharArray(_temp.mqtt_password, sizeof(_temp.mqtt_password));
  getMQTTTopic().toCharArray(_temp.mqtt_topic, sizeof(_temp.mqtt_topic));
  
  _temp.mqtt_port = getMQTTPort().toInt();
  _temp.temp_correction = getTemperatureCorrection().toFloat();
  _temp.temp_interval = getTemperatureInterval().toInt();

  return _temp;
}

String SonoffEEPROM::getWiFiSSID() {
  return read(232, 32);
}

String SonoffEEPROM::getWiFiPassword() {
  return read(264, 32);
}

String SonoffEEPROM::getMQTTHost() {
  return read(296, 32);
}

String SonoffEEPROM::getMQTTPort() {
  return read(328, 5);
}

String SonoffEEPROM::getMQTTTopic() {
  return read(397, 32);  
}

String SonoffEEPROM::getMQTTUser() {
  return read(333, 32);
}

String SonoffEEPROM::getMQTTPassword() {
  return read(365, 32);
}

String SonoffEEPROM::getSwitchMode() {
  return read(104, 1);
}

String SonoffEEPROM::getTemperatureCorrection() {
  return read(105, 5);
}

String SonoffEEPROM::getTemperatureInterval() {
  return read(110, 8);
}

unsigned int SonoffEEPROM::getRelayState() {
  return read(118, 1).toInt();
}

void SonoffEEPROM::saveSwitchMode(int in) {
  Serial << "Storing Switch Mode" << endl;
  write(104, 1, String(in));
}

void SonoffEEPROM::saveTemperatureCorrection(float in) {
  Serial << "Storing temperature correction" << endl;
  write(105, 5, String(in));
}

void SonoffEEPROM::saveTemperatureInterval(unsigned int in) {
  Serial << "Storing temperature interval" << endl;
  write(110, 8, String(in));
}

void SonoffEEPROM::saveRelayState(unsigned int in) {
  Serial << "Storing Relay state" << endl;
  write(118, 1, String(in));
}


void SonoffEEPROM::saveWiFiSSID(String in) {
  Serial << "Storing WiFi SSID" << endl;
  write(232, 32, in);
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  Serial << "Storing WiFi Password" << endl;
  write(264, 32, in);
}

void SonoffEEPROM::saveMQTTHost(String in) {
  Serial << "Storing MQTT Host" << endl;
  write(296, 32, in);
}

void SonoffEEPROM::saveMQTTPort(unsigned int in) {
  Serial << "Storing MQTT Port" << endl;
  write(328, 5, String(in));
}

void SonoffEEPROM::saveMQTTUser(String in) {
  Serial << "Storing MQTT User" << endl;
  write(333, 32, in);
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  Serial << "Storing MQTT Password" << endl;
  write(365, 32, in);
}

void SonoffEEPROM::saveMQTTTopic(String in) {
  Serial << "Storing MQTT Topic" << endl;
  write(397, 32, in);
}

void SonoffEEPROM::erase() {
  Serial << "Erasing EEPROM" << endl;
  clear(0, EEPROM_size);
  setDefaults();
}

void SonoffEEPROM::setDefaults() {
  Serial << "Setting default values" << endl;
  saveTemperatureCorrection(sonoffDefault.temp_correction);
  saveTemperatureInterval(sonoffDefault.temp_interval);
  saveMQTTPort(sonoffDefault.mqtt_port);
  saveMQTTTopic(sonoffDefault.mqtt_topic);
}

void SonoffEEPROM::write(int address, int size, String in) {
  clear(address, size);
  Serial << " - saveing [0x" << address << "] String size=" << in.length() << " : ";
  for (int i = 0; i < in.length(); ++i)
  {
    EEPROM.write(address + i, in[i]);
    Serial << in[i];
  }
  EEPROM.commit();
  Serial << " [completed]" << endl;
}

String SonoffEEPROM::read(int address, int size) {
  String _return;

  Serial << " - reading " << size << "B from EEPROM [0x" << address << "] : ";
  for (int i = address; i < address + size; ++i)
  {
    if (EEPROM.read(i) != 255) {
      //  Serial << char(EEPROM.read(i)) << "[" << EEPROM.read(i) << "],";
      _return += char(EEPROM.read(i));
    }
  }
  Serial << _return << endl;
  return _return;
}


/* Clear EEPROM */
void SonoffEEPROM::clear(int address, int size) {
  Serial << " - erasing " << size << "B from EEPROM [0x" << address << "]" << endl;
  for (int i = 0; i < size; ++i) {
    EEPROM.write(i + address, 255);
  }
  EEPROM.commit();
}

