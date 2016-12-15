#include "sonoff-eeprom.h"

SonoffEEPROM::SonoffEEPROM() {
  EEPROM.begin(512);
}

void SonoffEEPROM::read() {
  String wifi_ssid;
  String wifi_password;
  Serial << "Reading EEPROM" << endl;
  for (int i = 0; i < 32; ++i)
  {
    sonoffConfig.wifi_ssid[i] += EEPROM.read(i);
  }
  Serial << " - WIFI SSID: " << sonoffConfig.wifi_ssid << endl;
  for (int i = 32; i < 96; ++i)
  {
    sonoffConfig.wifi_password[i-32] += EEPROM.read(i);
  }
  Serial << " - WIFI Password: " << sonoffConfig.wifi_password << endl;
}


void SonoffEEPROM::saveWiFiSSID(String in) {
  Serial << "Storing WiFi SSID" << endl;
  write(0,32,in);   
}

void SonoffEEPROM::saveWiFiPassword(String in) {
  Serial << "Storing WiFi Password" << endl;
  write(32,64,in);  
}

void SonoffEEPROM::write(int address, int size, String in) {
  clear(address,size);
  Serial << " - saveing [0x" << address <<"]: ";
  for (int i = 0; i < in.length(); ++i)
  {
    EEPROM.write(address+i, in[i]);
    Serial << in[i];
  }
  EEPROM.commit();
  Serial << " [completed]" << endl;   
}

/* Clear EEPROM */
void SonoffEEPROM::clear(int start, int size) {
  Serial << " - erasing " << size << "B from EEPROM [0x" << start << "]" << endl;  
  for (int i = 0; i < size; ++i) {
    EEPROM.write(i+start, 0);
  }
  EEPROM.commit();
}

