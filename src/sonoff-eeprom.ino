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

  getVersion().toCharArray(_temp.version,sizeof(_temp.version));
  _temp.mode = getSwitchMode().toInt();
  
  // If thwew is no version in EPPROM this a first launch 
  if(_temp.version[0] == '\0')  {
    erase();
    _temp.mode = 1;
  }

  getID().toCharArray(_temp.id, sizeof(_temp.id));
  getHostName().toCharArray(_temp.host_name, sizeof(_temp.host_name));

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

String SonoffEEPROM::getVersion() {
  return read(0, 8);
}

String SonoffEEPROM::getID() {
  return read(119, 6);
}

String SonoffEEPROM::getHostName() {
  return read(125, 13);
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


void SonoffEEPROM::saveVersion(String in) {
  Serial << "Saving version" << endl;
  write(0, 8, in);
}

void SonoffEEPROM::saveSwitchMode(int in) {
  Serial << "Saving  Switch Mode" << endl;
  write(104, 1, String(in));
}

void SonoffEEPROM::saveTemperatureCorrection(float in) {
  Serial << "Saving  temperature correction" << endl;
  write(105, 5, String(in));
}

void SonoffEEPROM::saveTemperatureInterval(unsigned int in) {
  Serial << "Saving  temperature interval" << endl;
  write(110, 8, String(in));
}

void SonoffEEPROM::saveRelayState(unsigned int in) {
  Serial << "Saving  Relay state" << endl;
  write(118, 1, String(in));
}


void SonoffEEPROM::saveWiFiSSID(String in) {
  Serial << "Saving  WiFi SSID" << endl;
  write(232, 32, in);
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  Serial << "Saving  WiFi Password" << endl;
  write(264, 32, in);
}

void SonoffEEPROM::saveMQTTHost(String in) {
  Serial << "Saving  MQTT Host" << endl;
  write(296, 32, in);
}

void SonoffEEPROM::saveMQTTPort(unsigned int in) {
  Serial << "Saving  MQTT Port" << endl;
  write(328, 5, String(in));
}

void SonoffEEPROM::saveMQTTUser(String in) {
  Serial << "Saving  MQTT User" << endl;
  write(333, 32, in);
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  Serial << "Saving  MQTT Password" << endl;
  write(365, 32, in);
}

void SonoffEEPROM::saveMQTTTopic(String in) {
  Serial << "Saving  MQTT Topic" << endl;
  write(397, 32, in);
}

void SonoffEEPROM::erase() {
  Serial << "Erasing EEPROM" << endl;
  clear(0, EEPROM_size);
  setDefaults();
}

void SonoffEEPROM::setDefaults() {
  
  Serial << "Setting default values" << endl;


  char _id[6] = {0};
  char _host_name[13] = {0};
  char _mqtt_topic[32] = {0};
  
  sprintf(_id, "%06X", ESP.getChipId());
  sprintf(_host_name,"SONOFF_%s",_id);
  sprintf(_mqtt_topic,"/sonoff/%s/",_id);
  
  write(119, 6, _id);
  write(125, 13, _host_name);

  saveVersion(sonoffDefault.version);
  
  saveTemperatureCorrection(sonoffDefault.temp_correction);
  saveTemperatureInterval(sonoffDefault.temp_interval);
  saveMQTTTopic(_mqtt_topic);    
  saveMQTTPort(sonoffDefault.mqtt_port);

  saveSwitchMode(0);
  saveRelayState(0);  
  
}

void SonoffEEPROM::write(int address, int size, String in) {
  clear(address, size);
  Serial << " - saving [0x" << address << "] String size=" << in.length() << " : ";
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

