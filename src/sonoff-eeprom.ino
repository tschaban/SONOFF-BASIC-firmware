/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-eeprom.h"

SonoffEEPROM::SonoffEEPROM() {
  EEPROM.begin(EEPROM_size);

  /* If version not set it assumes first launch */
  if (read(0,8)[0] == '\0')  {
    erase();
    saveMode(1);
  }
}


SONOFFCONFIG SonoffEEPROM::getConfiguration() {
  SONOFFCONFIG _temp;

  read(0,8).toCharArray(_temp.version,sizeof(_temp.version));
  _temp.mode = read(104, 1).toInt();
  
  // If there is no version in EPPROM this a first launch 
  if(_temp.version[0] == '\0')  {
    erase();
    _temp.mode = MODE_ACCESSPOINT;
  }

  read(119, 6).toCharArray(_temp.id, sizeof(_temp.id));
  read(140, 2).toCharArray(_temp.language, sizeof(_temp.language));
  read(125, 13).toCharArray(_temp.host_name, sizeof(_temp.host_name));
  read(232, 32).toCharArray(_temp.wifi_ssid, sizeof(_temp.wifi_ssid));
  read(264, 32).toCharArray(_temp.wifi_password, sizeof(_temp.wifi_password));  
  read(296, 32).toCharArray(_temp.mqtt_host, sizeof(_temp.mqtt_host));   
  read(333, 32).toCharArray(_temp.mqtt_user, sizeof(_temp.mqtt_user));
  read(365, 32).toCharArray(_temp.mqtt_password, sizeof(_temp.mqtt_password));
  read(397, 32).toCharArray(_temp.mqtt_topic, sizeof(_temp.mqtt_topic));  
  
  _temp.ds18b20_present = (read(138,1).toInt()==1?true:false);
  
  _temp.ds18b20_correction = read(105, 5).toFloat();

  _temp.mqtt_port = read(328, 5).toInt(); 
  _temp.ds18b20_interval = read(110, 8).toInt();

  return _temp;
}


boolean SonoffEEPROM::isDS18B20Present() {
  return (read(138, 1).toInt() == 1 ? true : false);
}

float SonoffEEPROM::DS18B20Correction() {
  return read(105, 5).toFloat();
}

unsigned int SonoffEEPROM::DS18B20ReadInterval() {
  return read(110, 8).toInt();
}

uint8_t SonoffEEPROM::getRelayState() {
  return read(118, 1).toInt();
}

uint8_t SonoffEEPROM::getRelayStartState() {
  return read(139,1).toInt();
}

void SonoffEEPROM::saveVersion(String in) {
  write(0, 8, in);
}

void SonoffEEPROM::saveLanguage(String in) {
  write(140,2,in);
}
  
void SonoffEEPROM::saveMode(int in) {
  write(104, 1, String(in));
}

void SonoffEEPROM::saveTemperatureCorrection(float in) {
  write(105, 5, String(in));
}

void SonoffEEPROM::saveTemperatureInterval(unsigned int in) {
  write(110, 8, String(in));
}

void SonoffEEPROM::saveTemperatureSensorPresent(unsigned int in) {
  write(138, 1, String(in));
}

void SonoffEEPROM::saveRelayState(unsigned int in) {
  write(118, 1, String(in));
}

void SonoffEEPROM::saveRelayDefaultState(unsigned int in) {
  write(139, 1, String(in));
}

void SonoffEEPROM::saveWiFiSSID(String in) {
  write(232, 32, in);
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  write(264, 32, in);
}

void SonoffEEPROM::saveMQTTHost(String in) {
  write(296, 32, in);
}

void SonoffEEPROM::saveMQTTPort(unsigned int in) {
  write(328, 5, String(in));
}

void SonoffEEPROM::saveMQTTUser(String in) {
  write(333, 32, in);
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  write(365, 32, in);
}

void SonoffEEPROM::saveMQTTTopic(String in) {
  write(397, 32, in);
}

void SonoffEEPROM::erase() {
  clear(0, EEPROM_size);
  setDefaults();
}

void SonoffEEPROM::setDefaults() {
  
  char _id[6] = {0};
  char _host_name[13] = {0};
  char _mqtt_topic[32] = {0};

  sprintf(_id, "%06X", ESP.getChipId());
  sprintf(_host_name, "SONOFF_%s", _id);
  sprintf(_mqtt_topic, "/sonoff/%s/", _id);

  write(119, 6, _id);
  write(125, 13, _host_name);

  saveVersion(sonoffDefault.version);

  saveTemperatureCorrection(sonoffDefault.temp_correction);
  saveTemperatureInterval(sonoffDefault.temp_interval);
  saveTemperatureSensorPresent(sonoffDefault.temp_present);

  saveMQTTTopic(_mqtt_topic);
  saveMQTTPort(sonoffDefault.mqtt_port);

  saveMode(MODE_SWITCH);
  saveRelayState(0);
  saveRelayDefaultState(sonoffDefault.relay_post_crash);
  saveLanguage(sonoffDefault.language); 
}


void SonoffEEPROM::write(int address, int size, String in) {
  clear(address, size);
  for (int i = 0; i < in.length(); ++i)
  {
    EEPROM.write(address + i, in[i]);
  }
  EEPROM.commit();
}

String SonoffEEPROM::read(int address, int size) {
  String _return;
  for (int i = address; i < address + size; ++i)
  {
    if (EEPROM.read(i) != 255) {
      _return += char(EEPROM.read(i));
    }
  }
  return _return;
}

void SonoffEEPROM::clear(int address, int size) {
  for (int i = 0; i < size; ++i) {
    EEPROM.write(i + address, 255);
  }
  EEPROM.commit();
}

