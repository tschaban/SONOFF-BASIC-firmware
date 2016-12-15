/* Reading data from EEPROM */
void readFromEEPROM() {
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

/* Saveing data to EPPROM */
void saveToEEPROM(String ssid,String password) {
  Serial << "Saving to EEPROM" << endl;
  clearEEPROM();
  for (int i = 0; i < ssid.length(); ++i)
  {
    EEPROM.write(i, ssid[i]);
    Serial << ssid[i];
  } 
  Serial << endl;
   
  for (int i = 0; i < password.length(); ++i)
  {
    EEPROM.write(32+i, password[i]);
    Serial << password[i];
  }
  Serial << endl;
  EEPROM.commit();
  Serial << " - saved" << endl;
  readFromEEPROM();
}

/* Clear EEPROM */
void clearEEPROM() {
  Serial << " - erasing EEPROM" << endl;  
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

