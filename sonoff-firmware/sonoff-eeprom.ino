#include "sonoff-eeprom.h"

SonoffEEPROM::SonoffEEPROM() {
  EEPROM.begin(512);
}


SONOFFCONFIG SonoffEEPROM::getConfiguration() {
  Serial << "Reading configuration from EEPROM" << endl; 
  SONOFFCONFIG _temp;
  getWiFiSSID().toCharArray(_temp.wifi_ssid,sizeof(_temp.wifi_ssid));
  getWiFiPassword().toCharArray(_temp.wifi_password,sizeof(_temp.wifi_password));
  getMQTTHost().toCharArray(_temp.mqtt_host,sizeof(_temp.mqtt_host));
  _temp.mqtt_port = getMQTTPort().toInt();
  getMQTTUser().toCharArray(_temp.mqtt_user,sizeof(_temp.mqtt_user));
  getMQTTPassword().toCharArray(_temp.mqtt_password,sizeof(_temp.mqtt_password));

  return _temp;
}

String SonoffEEPROM::getWiFiSSID() {
  return read(232,32);
}

String SonoffEEPROM::getWiFiPassword() {
  return read(264,32);
}

String SonoffEEPROM::getMQTTHost() {
  return read(296,32);
}

String SonoffEEPROM::getMQTTPort() {
  return read(328,5);
}

String SonoffEEPROM::getMQTTUser() {
  return read(333,32);
}

String SonoffEEPROM::getMQTTPassword() {
  return read(365,32);
}

void SonoffEEPROM::saveWiFiSSID(String in) {
  Serial << "Storing WiFi SSID" << endl;
  write(232,32,in);   
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  Serial << "Storing WiFi Password" << endl;
  write(264,32,in);  
}

void SonoffEEPROM::saveMQTTHost(String in) {
  Serial << "Storing MQTT Host" << endl;
  write(296,32,in);   
}

void SonoffEEPROM::saveMQTTPort(String in) {
  Serial << "Storing MQTT Port" << endl;
  write(328,5,in);   
}

void SonoffEEPROM::saveMQTTUser(String in) {
  Serial << "Storing MQTT User" << endl;
  write(333,32,in);   
}

void SonoffEEPROM::saveMQTTPassword(String in) {
  Serial << "Storing MQTT Password" << endl;
  write(365,32,in);   
}


void SonoffEEPROM::write(int address, int size, String in) {
  clear(address,size);
  Serial << " - saveing [0x" << address <<"] String size=" << in.length()<< " : ";
  for (int i = 0; i < in.length(); ++i)
  {
    EEPROM.write(address+i, in[i]);
    Serial << in[i];
  }
  EEPROM.commit();
  Serial << " [completed]" << endl;   
}

String SonoffEEPROM::read(int address, int size) {
  String _return;
  
  Serial << " - reading " << size << "B from EEPROM [0x" << address << "] : "; 
  for (int i = address; i < address+size; ++i)
  {
   if (EEPROM.read(i)!=255) {
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
    EEPROM.write(i+address, 255);
  }
  EEPROM.commit();
}

