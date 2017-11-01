/*
  SONOFF BASIC: firmware
  More info: https://github.com/tschaban/SONOFF-BASIC-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

#include "sonoff-upgrade.h"

SonoffFirmware::SonoffFirmware() {}

boolean SonoffFirmware::upgraded() {
  if (String(sonoffDefault.version) != String(Configuration.version)) {
    return true;
  } else {
    return false;
  }
}

void SonoffFirmware::update() {
  if (Configuration.debugger)
    Serial << endl
           << "WARN: Firmware was upgraded from version : "
           << Configuration.version << " to " << sonoffDefault.version;
  Eeprom.saveVersion(sonoffDefault.version);

  v032();
  v040();
  v050();
  v061();
  v070();
  v089();
  v010dh();

  Configuration = Eeprom.getConfiguration();
}

void SonoffFirmware::v032() {
  if (String(Configuration.version) == "0.3.2") {
    Eeprom.saveRelayStateAfterPowerRestored(
        sonoffDefault.relay_state_after_power_restored);
  }
}

void SonoffFirmware::v040() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0") {
    Eeprom.saveLanguage(sonoffDefault.language);
  }
}
void SonoffFirmware::v050() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0" ||
      String(Configuration.version) == "0.5.0") {

    char _id[6] = {0};
    char _device_name[32] = {0};

    sprintf(_id, "%06X", ESP.getChipId());
    sprintf(_device_name, "SONOFF_%s", _id);
    Eeprom.saveDeviceName(_device_name);

    /* After Connecton is restored parameter is set to saved After Power is
     * restored value */
    Eeprom.saveRelayStateAfterConnectionRestored(
        Eeprom.getRelayStateAfterPowerRestored());

    /* Value "Server value" [4] is not longer needed for parameter  After Power
       Restored. If it was set like that  by a user it's upgraded to value 2
       which is relay state OFF
    */
    if (Eeprom.getRelayStateAfterPowerRestored() == 4) {
      Eeprom.saveRelayStateAfterPowerRestored(2);
    }
  }
}

void SonoffFirmware::v061() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0" ||
      String(Configuration.version) == "0.5.0" ||
      String(Configuration.version) == "0.6.1") {
    Eeprom.saveSwitchPresent(sonoffDefault.switch_present);
    Eeprom.saveSwitchGPIO(sonoffDefault.switch_gpio);
    Eeprom.saveSwitchSensitiveness(sonoffDefault.switch_sensitiveness);
    Eeprom.saveDebuggable(0);
  }
}

void SonoffFirmware::v070() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0" ||
      String(Configuration.version) == "0.5.0" ||
      String(Configuration.version) == "0.6.1" ||
      String(Configuration.version) == "0.7.0") {
    Eeprom.saveInterface(1);
    Eeprom.saveNumberConnectionAttempts(
        sonoffDefault.number_connection_attempts);
    Eeprom.saveDurationBetweenConnectionAttempts(
        sonoffDefault.duration_between_connection_attempts);
    Eeprom.saveDurationBetweenNextConnectionAttemptsSeries(
        sonoffDefault.duration_between_next_connection_attempts_series);
  }
}

// version 0.8.9 introduced switch_type
void SonoffFirmware::v089() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0" ||
      String(Configuration.version) == "0.5.0" ||
      String(Configuration.version) == "0.6.1" ||
      String(Configuration.version) == "0.7.0" ||
      String(Configuration.version) == "0.8.7" ||
      String(Configuration.version) == "0.8.8") {
    Eeprom.saveSwitchType(sonoffDefault.switch_type);
  }
}

void SonoffFirmware::v010dh() {
  if (String(Configuration.version) == "0.3.2" ||
      String(Configuration.version) == "0.4.0" ||
      String(Configuration.version) == "0.5.0" ||
      String(Configuration.version) == "0.6.1" ||
      String(Configuration.version) == "0.7.0" ||
      String(Configuration.version) == "0.8.7" ||
      String(Configuration.version) == "0.8.8" ||
      String(Configuration.version) == "0.8.9") {
    Eeprom.saveTemperatureSensorType(1); // DH11
  }
}
