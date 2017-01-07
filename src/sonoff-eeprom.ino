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
  _temp.mode = read(104, 1).toInt();
  
  // If thwew is no version in EPPROM this a first launch 
  if(_temp.version[0] == '\0')  {
    erase();
    _temp.mode = 1;
  }

  read(119, 6).toCharArray(_temp.id, sizeof(_temp.id));
  read(125, 13).toCharArray(_temp.host_name, sizeof(_temp.host_name));

  read(232, 32).toCharArray(_temp.wifi_ssid, sizeof(_temp.wifi_ssid));
  read(264, 32).toCharArray(_temp.wifi_password, sizeof(_temp.wifi_password));
  
  read(296, 32).toCharArray(_temp.mqtt_host, sizeof(_temp.mqtt_host));
  _temp.mqtt_port = read(328, 5).toInt();  
  read(333, 32).toCharArray(_temp.mqtt_user, sizeof(_temp.mqtt_user));
  read(365, 32).toCharArray(_temp.mqtt_password, sizeof(_temp.mqtt_password));
  read(397, 32).toCharArray(_temp.mqtt_topic, sizeof(_temp.mqtt_topic));
  
  _temp.temp_present = (read(138,1).toInt()==1?true:false);
  _temp.temp_correction = read(105, 5).toFloat();
  _temp.temp_interval = read(110, 8).toInt();

  return _temp;
}

String SonoffEEPROM::getVersion() {
  return read(0, 8);
}

unsigned int SonoffEEPROM::getRelayState() {
  return read(118, 1).toInt();
}

void SonoffEEPROM::saveVersion(String in) {
  Serial << endl << "Saving version" << endl;
  write(0, 8, in);
}

void SonoffEEPROM::saveSwitchMode(int in) {
  Serial << endl << "Saving  Switch Mode" << endl;
  write(104, 1, String(in));
}

void SonoffEEPROM::saveTemperatureCorrection(float in) {
  Serial << endl << "Saving  temperature correction" << endl;
  write(105, 5, String(in));
}

void SonoffEEPROM::saveTemperatureInterval(unsigned int in) {
  Serial << endl << "Saving  temperature interval" << endl;
  write(110, 8, String(in));
}

void SonoffEEPROM::saveTemperatureSensorPresent(unsigned int in) {
  Serial << endl << "Saving ds18b20 presence" << endl;
  write(138, 1, String(in));
}

void SonoffEEPROM::saveRelayState(unsigned int in) {
  Serial << endl << "Saving  Relay state" << endl;
  write(118, 1, String(in));
}

void SonoffEEPROM::saveWiFiSSID(String in) {
  Serial << endl << "Saving  WiFi SSID" << endl;
  write(232, 32, in);
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  Serial << endl << "Saving  WiFi Password" << endl;
  write(264, 32, in);
}

void SonoffEEPROM::saveMQTTHost(String in) {
  Serial << endl << "Saving  MQTT Host" << endl;
  write(296, 32, in);
}

void SonoffEEPROM::saveMQTTPort(unsigned int in) {
  Serial << endl << "Saving  MQTT Port" << endl;
  write(328, 5, String(in));
}

void SonoffEEPROM::saveMQTTUser(String in) {
  Serial << endl << "Saving  MQTT User" << endl;
  write(333, 32, in);
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  Serial << endl << "Saving  MQTT Password" << endl;
  write(365, 32, in);
}

void SonoffEEPROM::saveMQTTTopic(String in) {
  Serial << endl << "Saving  MQTT Topic" << endl;
  write(397, 32, in);
}

void SonoffEEPROM::erase() {
  Serial << "Erasing EEPROM" << endl;
  clear(0, EEPROM_size);
  setDefaults();
}

void SonoffEEPROM::setDefaults() {  
  Serial << endl << "Setting default values" << endl;

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
  saveTemperatureSensorPresent(sonoffDefault.temp_present);
  
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

void SonoffEEPROM::clear(int address, int size) {
  Serial << " - erasing " << size << "B from EEPROM [0x" << address << "]" << endl;
  for (int i = 0; i < size; ++i) {
    EEPROM.write(i + address, 255);
  }
  EEPROM.commit();
}

