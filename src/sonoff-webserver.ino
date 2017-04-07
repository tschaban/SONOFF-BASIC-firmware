/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

void startHttpServer() {
  if (Configuration.debugger) Serial << endl << " - Starting web server" << endl;
  server.on("/", handleRoot);
  server.on("/configure", handleConfiguration);
  server.on("/reboot", handleReboot);
  server.on("/reset", handleReset);
  server.on("/save", handleSave);
  server.on("/favicon.ico",handleFavicon);
  server.onNotFound(handleNotFound);
  httpUpdater.setup(&server);
  server.begin();
  if (Configuration.debugger) Serial << " - Web server is working" << endl;
}

void handleRoot() {
  if (Configuration.debugger) Serial << "Server: root requested" << endl;
  if (Configuration.debugger) Serial << Configuration.language[0];
  String page = 
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Device information":"Informacje o urządzeniu";page+="</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">ID</td>"
    "<td>: ";page+=Configuration.id;page+="</td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Name":"Nazwa";page+="</td>"
    "<td>: ";page+=Configuration.device_name;page+="</td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">Firmware:</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Version":"Wersja";page+="</td>"
    "<td>: ";page+=Configuration.version;page+="</td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">WiFi:</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">SSID</td>"
    "<td>: ";page+=Configuration.wifi_ssid;page+="</td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">MQTT Broker</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">Host</td>"
    "<td>: ";page+=Configuration.mqtt_host;page+=":";  page += Configuration.mqtt_port;page+="</td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"MQTT Topic":"Temat";page+="</td>"
    "<td>: ";page+=Configuration.mqtt_topic;page+="</td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Temperature sensor":"Czujnik temperatury";page+="</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Present?":"Podłączony?";page+="</td>"
    "<td>: "; 
    page += Configuration.ds18b20_present?Configuration.language[0]==101?"Yes":"Tak":Configuration.language[0]==101?"No":"Nie";
    page += "</td>"
    "</tr>";
    if (Configuration.ds18b20_present) {
      page += 
      "<tr>"
      "<td class=\"label\">";page+=Configuration.language[0]==101?"Correction":"Korekta";page+="</td>"
      "<td>: ";page+=Configuration.ds18b20_correction;page+="</td>"
      "</tr>"
      "<tr>"
      "<td class=\"label\">";page+=Configuration.language[0]==101?"Interval":"Częstotliwość odczytu";page+="</td>"
      "<td>: ";page+=Configuration.ds18b20_interval;page+="s.</td>"
      "</tr>";
    }
    page += "</table>"
    "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"External switch":"Zewnętrzny przełącznik";page+="</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Connected?":"Podłączony?";page+="</td>"
    "<td>: "; 
    page += Configuration.switch_present?Configuration.language[0]==101?"Yes":"Tak":Configuration.language[0]==101?"No":"Nie";
    page += "</td>"
    "</tr>";
    if (Configuration.switch_present) {
      page += 
      "<tr>"
      "<td class=\"label\">GPIO</td>"
      "<td>: ";page+=Configuration.switch_gpio;page+="</td>"
      "</tr>";
    }
    page += "</table>"
    "</div>"

    
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Relay":"Przekaźnik";page+="</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"After power is restored relay is to":"Po przywróceniu zasilania przekaźnik jest ustawiony na";page+="</td>"
    "<td>:  "; 

    if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_ON) {
        page += Configuration.language[0]==101?"ON":"Włączony";
    } else if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_OFF) {
        page += Configuration.language[0]==101?"OFF":"Wyłączony";
    } else if (Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_LAST_KNOWN) {
        page += Configuration.language[0]==101?"Last known":"Ostatnia zapamiętana wartość";
    }
    
    page += "</td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"After connection to MQTT Broker is restored relay is set to":"Po przywróceniu połączenia do brokera MQTT, przekaźnik jest ustawiony na";page+="</td>"
    "<td>: "; 

    if (Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_ON) {
        page += Configuration.language[0]==101?"ON":"Włączony";
    } else if (Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_OFF) {
        page += Configuration.language[0]==101?"OFF":"Wyłączony";
    } else if (Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_LAST_KNOWN) {
        page += Configuration.language[0]==101?"Last known":"Ostatnią zapamiętaną wartość";
    } else if (Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_SERVER) {
        page += Configuration.language[0]==101?"Server value":"Wartość z serwera";
    }
    
    page += "</td>"
    "</tr>"    
    "</table>"
    "</div>";

  generatePage(page,true,0);
}

void handleConfiguration() {

  if (Configuration.debugger) Serial << "Server: configuration" << endl;
  
  String page =
    "<form action=\"/save\"  method=\"post\">"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Language":"Język";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Select language":"Wybierz język";page+=":<sup class=\"red\">*</sup></td>"
    "<td>: <select name=\"language\" length=1>"
    "<option value=\"en\"";page+=Configuration.language[0]==101?" selected=\"selected\"":"";page+=">English</option>"
    "<option value=\"pl\"";page+=Configuration.language[0]!=101?" selected=\"selected\"":"";page+=">Polski</option>"
    "</select></td>"
    "</tr>"
    "</table>"   
    "</div>"  
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Device name":"Nazwa przełącznika";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Name":"Nazwa";page+="<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"device_name\" length=32 value=\"";page+=Configuration.device_name;page+="\" /></td>"
    "</tr>"
    "</table>"      
    "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"WiFi configuration":"Konfiguracja WiFi";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">SSID<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"wifi_ssid\" length=32 value=\"";page+=Configuration.wifi_ssid;page+="\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Password":"Hasło";page+="<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"wifi_password\" length=32 value=\"";page+=Configuration.wifi_password;page+="\" /></td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Controlling interface":"Interface sterowania";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">Interface<sup class=\"red\">*</sup></td>"
    "<td>: <select name=\"interface\" length=1>"
    "<option value=\"0\"";page+=Configuration.interface==0?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"None / Manual":"Brak / Ręczny";page+="</option>"
    "<option value=\"1\"";page+=Configuration.interface==1?" selected=\"selected\"":"";page+=">MQTT</option>"
    "<option value=\"2\"";page+=Configuration.interface==2?" selected=\"selected\"":"";page+=">HTTP</option>"
    "</select></td>"
    "</tr>"
    "</table>"   
    "</div>"      
    "<div class=\"section\">";page+=Configuration.language[0]==101?"MQTT Broker configuration":"Konfiguracja MQTT Brokera";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">Host<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"mqtt_host\" length=32 value=\"";page+=Configuration.mqtt_host;page+="\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">Port<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"number\" name=\"mqtt_port\" length=5 value=\"";page+=Configuration.mqtt_port;page+="\"/></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"User":"Użytkownik";page+="</td>"
    "<td>: <input type=\"text\" name=\"mqtt_user\" length=32 value=\"";page+=Configuration.mqtt_user;page+="\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Password":"Hasło";page+="</td>"
    "<td>: <input type=\"text\" name=\"mqtt_password\" length=32 value=\"";page+=Configuration.mqtt_password;page+="\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Topic":"Temat";page+="<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"mqtt_topic\" length=32 value=\"";page+=Configuration.mqtt_topic;page+="\" /></td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Temperature sensor configuration":"Konfiguracja czujnika temperatury";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Connected":"Podłączony";page+="?<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"checkbox\" name=\"temp_present\" length=1 value=\"1\"";
  if (Eeprom.isDS18B20Present()) {
    page += " checked ";
  }

  page += "\" /></td>"
          "</tr>"
          "<tr>"
          "<td class=\"label\">";page+=Configuration.language[0]==101?"Correction":"Korekta temperatury";page+="</td>"
          "<td>: <input type=\"text\" name=\"temp_correction\" length=5 value=\"";page+=Configuration.ds18b20_correction;page+="\" /></td>"
          "</tr>"
          "<tr>"
          "<td class=\"label\">";page+=Configuration.language[0]==101?"Interval (in sec)":"Częstotliwość odczytu (w sek)";page+="</td>"
          "<td>: <input type=\"text\" name=\"temp_interval\" length=8 value=\"";page+=Configuration.ds18b20_interval;page+="\" /></td>"
          "</tr>"
          "</table>";
          
   page += "</div>"
    "<div class=\"section\">";page+=Configuration.language[0]==101?"External switch configuration":"Konfiguracja zewnętrznego przełącznika";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Connected":"Podłączony";page+="?<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"checkbox\" name=\"switch_present\" length=1 value=\"1\"";
  if (Eeprom.isSwitchPresent()) {
    page += " checked ";
  }

  page += "\" /></td>"
          "</tr>"
          "<tr>"
          "<td class=\"label\">GPIO</td>"
          
          "<td>: <select name=\"switch_gpio\" length=2>"
          "<option value=\"1\""; page+=Eeprom.getSwitchGPIO()==GPIO_1 ?" selected=\"selected\"":"";page+=">GPIO 1</option>"
          "<option value=\"3\""; page+=Eeprom.getSwitchGPIO()==GPIO_3 ?" selected=\"selected\"":"";page+=">GPIO 3</option>"
          "<option value=\"14\""; page+=Eeprom.getSwitchGPIO()==GPIO_14 ?" selected=\"selected\"":"";page+=">GPIO 14</option>"
          "</select></td>"
          
          "</tr>"
          "<tr>"
          "<td class=\"label\">";page+=Configuration.language[0]==101?"Sensitiveness":"Czułość";page+="</td>"
          "<td>: <select name=\"switch_sensitiveness\" length=1>"
          "<option value=\"0\""; page+=Eeprom.getSwitchSensitiveness()==SWITCH_SENSITIVENESS_HIGH ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"High":"Wysoka";page+="</option>"
          "<option value=\"1\""; page+=Eeprom.getSwitchSensitiveness()==SWITCH_SENSITIVENESS_NORMAL ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Suggested":"Zalecana";page+="</option>"
          "<option value=\"2\""; page+=Eeprom.getSwitchSensitiveness()==SWITCH_SENSITIVENESS_LOW ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Low":"Niska";page+="</option>"
          "</select></td>"
          "</tr>"
          "</table>";
          
   page += "</div>"
   
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Relay configuration":"Konfiguracja przekaźnika";page+=":</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"After power is restored set it to":"Po przywróceniu zasilania ustawić go na";page+="<sup class=\"red\">*</sup></td>"
    "<td>: <select name=\"relay_power_restored\" length=1>"
    "<option value=\"1\""; page+=Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_ON ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"On":"Włączony";page+="</option>"
    "<option value=\"2\""; page+=Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_OFF ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Off":"Wyłączony";page+="</option>"
    "<option value=\"3\""; page+=Eeprom.getRelayStateAfterPowerRestored()==DEFAULT_RELAY_LAST_KNOWN ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Last known":"Ostatnia zapamiętana wartość";page+="</option>"
    "</select></td>"
    "</tr>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"After connection to MQTT server is restored set it to":"Po przwróceniu połączenia do brokera MQTT ustawić go na";page+="<sup class=\"red\">*</sup></td>"
    "<td>: <select name=\"relay_connection_restored\" length=1>"
    "<option value=\"1\""; page+=Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_ON ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"On":"Włączony";page+="</option>"
    "<option value=\"2\""; page+=Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_OFF ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Off":"Wyłączony";page+="</option>"
    "<option value=\"3\""; page+=Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_LAST_KNOWN ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Last known":"Ostatnią zapamiętaną wartość";page+="</option>"
    "<option value=\"4\""; page+=Eeprom.getRelayStateAfterConnectionRestored()==DEFAULT_RELAY_SERVER ?" selected=\"selected\"":"";page+=">";page+=Configuration.language[0]==101?"Server value":"Wartość z serwera";page+="</option>"
    "</select></td>"
    "</tr>"
    "</table>"
    "</div>"
    "<div class=\"section\">Debugger:</div>"
    "<div class=\"section-content\">"
    "<table>"
    "<tr>"
    "<td class=\"label\">";page+=Configuration.language[0]==101?"Debugger on":"Debugger włączony";page+="?<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"checkbox\" name=\"debugger\" length=1 value=\"1\"";
  if (Eeprom.debuggable()) {
    page += " checked ";
  }

  page += "\" /></td>"    
    "</tr>"
    "</table>"
    "</div>"  


    
    "<div class=\"section\"></div>"
    "<div class=\"section-content\">";
  page += "<input class=\"submit\" type=\"submit\" value=\"";page+=Configuration.language[0]==101?"Save":"Zapisz";page+="\" />"
      "</div>";

  generatePage(page,true,0);
}

void handleSave() {
  Led.stopBlinking();
  Led.on();

  if (Configuration.debugger) Serial << "Server: saving data" << endl;

  if (server.arg("device_name").length() > 0) {
    Eeprom.saveDeviceName(server.arg("device_name"));
  }

  if (server.arg("wifi_ssid").length() > 0) {
    Eeprom.saveWiFiSSID(server.arg("wifi_ssid"));
  }

  if (server.arg("wifi_password").length() > 0) {
    Eeprom.saveWiFiPassword(server.arg("wifi_password"));
  }

  if (server.arg("mqtt_host").length() > 0) {
    Eeprom.saveMQTTHost(server.arg("mqtt_host"));
  }

  if (server.arg("mqtt_port").length() > 0) {
    Eeprom.saveMQTTPort(server.arg("mqtt_port").toInt());
  }

  if (server.arg("mqtt_user").length() > 0) {
    Eeprom.saveMQTTUser(server.arg("mqtt_user"));
  }

  if (server.arg("mqtt_password").length() > 0) {
    Eeprom.saveMQTTPassword(server.arg("mqtt_password"));
  }

  if (server.arg("mqtt_topic").length() > 0) {
    Eeprom.saveMQTTTopic(server.arg("mqtt_topic"));
  }

  if (server.arg("temp_present").length() > 0 ) {
    Eeprom.saveTemperatureSensorPresent(1);
  } else {
    if (Eeprom.isDS18B20Present()) {
      Eeprom.saveTemperatureSensorPresent(0);
    }
  }

  if (server.arg("temp_correction").length() > 0 ) {
    Eeprom.saveTemperatureCorrection(server.arg("temp_correction").toFloat());
  }

  if (server.arg("temp_interval").length() > 0) {
    Eeprom.saveTemperatureInterval(server.arg("temp_interval").toInt());
  }

  if (server.arg("relay_power_restored").length() > 0) {
    Eeprom.saveRelayStateAfterPowerRestored(server.arg("relay_power_restored").toInt());
  }

  if (server.arg("relay_connection_restored").length() > 0) {
    Eeprom.saveRelayStateAfterConnectionRestored(server.arg("relay_connection_restored").toInt());
  }

  if (server.arg("interface").length() > 0) {
    Eeprom.saveInterface(server.arg("interface").toInt());
  }

  if (server.arg("language").length() > 0) {
    Eeprom.saveLanguage(server.arg("language"));
  }

  if (server.arg("switch_present").length() > 0 ) {
    Eeprom.saveSwitchPresent(1);
  } else {
    if (Eeprom.isSwitchPresent()) {
      Eeprom.saveSwitchPresent(0);
    }
  }

  if (server.arg("switch_gpio").length() > 0 ) {
    Eeprom.saveSwitchGPIO(server.arg("switch_gpio").toInt());
  }

  if (server.arg("switch_sensitiveness").length() > 0 ) {
    Eeprom.saveSwitchSensitiveness(server.arg("switch_sensitiveness").toInt());
  }

  if (server.arg("debugger").length() > 0 ) {
    Eeprom.saveDebuggable(1);
  } else {
    if (Eeprom.debuggable()) {
      Eeprom.saveDebuggable(0);
    }
  }
  

  Configuration = Eeprom.getConfiguration();

  String page =
    "<div class=\"section-content\">"  
    "<h3 style=\"margin: 60px 0px;\">";page+=Configuration.language[0]==101?"Configuration has been successfully saved":"Konfiguracja została zapisana";page+="</h3>"
    "<p style='margin-bottom:70px;'><a href=\"/reboot\">";page+=Configuration.language[0]==101?"Exit":"Wyjdź";page+="</a> ";page+=Configuration.language[0]==101?"configuration to apply all changes":"z konfiguracji, aby zastosować wykonane zmiany";page+="</p>"
    "</div>";


  generatePage(page,true,0);

  Led.startBlinking(0.1);

}

void handleUpgrade() {
    if (Configuration.debugger) Serial << "Server: upgrade" << endl;
    String page =
      "<div class=\"section\">";page+=Configuration.language[0]==101?"Firmware upgrade":"Aktualizacja oprogramowania";page+="</div>"
      "<div class=\"section-content\">"
      "<p style='margin-top:40px;'><strong><span class=\"red\">";page+=Configuration.language[0]==101?"Important":"Ważne";page+=":</span> ";page+=Configuration.language[0]==101?"Do not disconnect Sonoff from power source during upgrade":"Nie odłączaj przełącznika od zasialania podczas aktualizacji oprogramowania";page+=".</strong></p>"
      "<p style='margin-bottom:40px;'>";page+=Configuration.language[0]==101?"After upgrade Sonoff will be automatically restarted":"Po aktualizacji, przełącznik zostanie automatycznie zresetowany";page+=". </p>"
      "<form style='margin-bottom: 100px;' method='POST' action='' enctype='multipart/form-data'>"
      "<input class='file' type='file' name='update' value='";page+=Configuration.language[0]==101?"Select firmware file":"Wybierz firmware";page+="'><br>"
      "<input class='submit' type='submit' value='";page+=Configuration.language[0]==101?"Upgrade":"Wgraj";page+="'>"
      "</div>"
      "</form>";
      
       generatePage(page,true,0);
}

void handleUpgradeCompleted(boolean status) {
    
    String page =
      "<div class=\"section\">";page+=Configuration.language[0]==101?"Firmware upgrade":"Aktualizacja oprogramowania";page+=":</div>"
      "<div class=\"section-content\">"
      "<div style='margin:60px 0'>";
  
      
      
      page += status ? (Configuration.language[0]==101?"<h3 class=\"red\">Upgrade failed</h3>":"<h3 class=\"red\">Aktualizacja nie powiodła się</h3>") : 
                       (Configuration.language[0]==101?"<h3>Upgrade was successful!</h3><p>After 10 seconds switch will be autmatically reloaded. Please wait.</p>"
                                                      :"<h3>Aktualizacja zakończona pomyślnie!</h3><p>Po 10 sekundach przełącznik zostanie przeładowany z wgranym oprogramowaniem. Proszę czekać.</p>");

      page += "</div></div>";
      
    generatePage(page, false, 10);
}

void handleNotFound() {
  if (Configuration.debugger) Serial << "Server: page not found" << endl;

  String page = 
    "<div class=\"section-content\">"  
    "<h4 style=\"margin: 60px 0px;\"><span class=\"red\">";page+=Configuration.language[0]==101?"Error":"Błąd";page+=" 404:</span> ";page+=Configuration.language[0]==101?"Page Not Found":"Strona nie została odnaleziona";page+=".</h4>"
    "</div>";

  if (Configuration.debugger) Serial << server.uri() << " " << server.args() << endl;
  
  for (uint8_t i = 0; i < server.args(); i++) {
   if (Configuration.debugger) Serial <<  server.argName(i) << ": " << server.arg(i) << endl;
  }
  generatePage(page,true,0);
}

void handleReboot() {
  if (Configuration.debugger) Serial << "Server: rebooting device" << endl;
  String page =
    "<div class=\"section-content\">"  
    "<h4 style=\"margin: 60px 0px;\">";page+=Configuration.language[0]==101?"Rebooting is in progress":"Trwa restart";page+="</h4>"
    "</div>";
  generatePage(page,false,10);
  Sonoff.toggle();
}

void handleReset() {
  String page =
    "<div class=\"section\">";page+=Configuration.language[0]==101?"Device reset":"Restart przeĹ‚Ä…cznika";page+="</div>"
    "<div class=\"section-content\">"  
    "<h4 style=\"margin: 40px 0 10px 0;\"><span class=\"red\">";page+=Configuration.language[0]==101?"Important":"WaĹĽne";page+=": </span> ";page+=Configuration.language[0]==101?"Sonoff will be to its default values. You will loose connecton with Sonoff":"Sonoff zostanie zresetowany do wartości początkowych. Utracisz z nim połączenie.";page+=".</h4>"
    "<p>";page+=Configuration.language[0]==101?"Connect to WiFi":"Podłacz się do WiFi o nazwie ";page+=": <strong>";page+=Configuration.device_name;page+="</strong> ";page+=Configuration.language[0]==101?"from your computer or smartphone then open":"ze swojego komputera lub smartfonu, a następnie otwórz ";page+=" "
    "<a href=\"http://192.168.5.1\">http://192.168.5.1</a>"
    " ";page+=Configuration.language[0]==101?"and finish configuration":"i dokończ konfigurację";page+=".</p><br /><br /><br /><br />"
    "</div>";
  generatePage(page,false,10);
  Sonoff.reset();
}

void handleFavicon() {
}


void generatePage(String &page, boolean navigation, uint8_t redirect) {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String _page = 
  "<!DOCTYPE html>"
  "<html lang=\"en\">"
  "<head>"
  "<meta charset=\"UTF-8\">"
  "<meta name=\"viewport\" content=\"user-scalable = yes\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

  if (redirect>0) {
    _page+="<meta http-equiv=\"refresh\" content=\"";
    _page+=String(redirect); 
    _page+=";URL=/\">";
  }
  
  _page+="<title>Sonoff Configuration</title>"
  "<style>"
  "body{font-family:Helvetica;font-size:14px;margin:0;background-color:#292929}a{color:#aaaaaf;text-decoration:none}a:hover{color:#fafaff}ul{margin:0;padding:0;list-style-type:none}li.horizontal{display:inline;padding-right:5px;color:#aaaaaf}.container{margin:0 auto;height:auto}.header,.navigation,.section,.section-content{width:100%}.header{color:#eaeaef;padding:10px}.navigation{background-color:#676767;color:#fafaff;padding:10px}.section{background-color:#f0f0f0;color:#292929;font-weight:bold;padding:10px}.section-content{background-color:#fffff0;padding:10px 0 10px 20px}.label{font-weight:bold}.submit,.file{margin:5px 0;padding:5px 50px;background-color:#060;border:1px solid #060;color:#fff}.red{color:red}.file{margin:5px 0;padding:5px;background-color:#008;border:1px solid #008;color:#fff}.submit:hover{background-color:#080}"
  "</style>"
  "</head>"
  "<body>"
  "<div class=\"container\">"
  "<div class=\"header\">"
  "<a href=\"/\"><img src=\"data:image/png;base64,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\"></a> BASIC"
  "</div>";

  if (navigation) {
    _page+=
    "<div class=\"navigation\">"
    "<ul>"
    "<li class=\"horizontal\"><a href=\"/configure\">";_page+=Configuration.language[0]==101?"Configuration":"Konfiguracja";_page+="</a> |</li>"
    "<li class=\"horizontal\"><a href=\"/update\">";_page+=Configuration.language[0]==101?"Upgrade":"Aktualizacja";_page+="</a> |</li>"
    "<li class=\"horizontal\"><a href=\"/reset\">";_page+=Configuration.language[0]==101?"Reset to factory settings":"Ustawienia fabryczne";_page+="</a> |</li>"
    "<li class=\"horizontal\"><a href=\"/reboot\">";_page+=Configuration.language[0]==101?"End configuration":"Zakończ konfigurację";_page+="</a></li>"
    "</ul>"
    "</div>";
  }

  _page+=page;
  
  _page+="<div class=\"header\"><p>"; 
  _page+=Configuration.language[0]==101?"Firmware version":"Wersja firmware"; 
  _page+=": " + String(Configuration.version) + "</p>"
  "<ul>";
  _page+=Configuration.language[0]==101 ? 
      "<li><a href=\"http://smart-house.adrian.czabanowski.com/en-sonoff-firmware/\" target=\"_blank\">Documentation</a></li>" :
      "<li><a href=\"http://smart-house.adrian.czabanowski.com/firmware-sonoff/\" target=\"_blank\">Dokumentacja</a></li>";

  _page+="<li><a href=\"http://smart-house.adrian.czabanowski.com/forum/firmware-do-przelacznika-sonoff/\" target=\"_blank\">"; 
  _page+=Configuration.language[0]==101?"Support":"Wsparcie";_page+="</a></li>";
        
  _page+="<li><a href=\"https://github.com/tschaban/SONOFF-firmware\" target=\"_blank\">GitHub</a></li>"
  
  "<li><a href=\"https://github.com/tschaban/SONOFF-BASIC-firmware/blob/master/LICENSE\" target=\"_blank\">";   
  _page+=Configuration.language[0]==101?"MIT License":"Licencja użytkownika";page+="</a></li>"
  "</ul>"
  "</div>"
  "</div>"
  "</body>"
  "</html>";
  server.send(200, "text/html", _page);
}





