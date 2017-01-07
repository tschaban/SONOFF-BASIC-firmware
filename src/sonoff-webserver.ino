/*
  Sonoff: firmware
  More info: https://github.com/tschaban/SONOFF-firmware
  LICENCE: http://opensource.org/licenses/MIT
  2016-10-27 tschaban https://github.com/tschaban
*/

void startHttpServer() {
  Serial << endl << " - Starting web server" << endl;
  server.on("/", handleRoot);
  server.on("/configure", handleConfiguration);
  server.on("/reboot", handleReboot);
  server.on("/reset", handleReset);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  httpUpdater.setup(&server);
  server.begin();
  Serial << " - Web server is working" << endl;
}

void handleRoot() {
  Serial << "Server: root requested" << endl;

  String page =
    "<h3>Device:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">ID</td>"
    "<td>: "; page += sonoffConfig.id; page += "</td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">Name</td>"
    "<td>: "; page += sonoffConfig.host_name; page += "</td>"
    "</tr>"
    "</table>"
    "<h3>Firmware:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">Version</td>"
    "<td>: "; page += sonoffConfig.version; page += "</td>"
    "</tr>"
    "</table>"
    "<h3>WiFi:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">WiFi SSID</td>"
    "<td>: "; page += sonoffConfig.wifi_ssid; page += "</td>"
    "</tr>"
//    "<tr>"
//    "<td class=\"header\">WiFi Password</td>"
//    "<td>: "; page += sonoffConfig.wifi_password; page += "</td>"
//    "</tr>"
    "</table>"
    "<h3>MQTT Broker:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">MQTT Host</td>"
    "<td>: "; page += sonoffConfig.mqtt_host; page += ":";  page += sonoffConfig.mqtt_port; page += "</td>"
    "</tr>"
    "<tr>"
//    "<td class=\"header\">MQTT User</td>"
//    "<td>: "; page += sonoffConfig.mqtt_user; page += "</td>"
//    "</tr>"
//    "<tr>"
//    "<td class=\"header\">MQTT Password</td>"
//    "<td>: "; page += sonoffConfig.mqtt_password; page += "</td>"
//    "</tr>"
    "<tr>"
    "<td class=\"header\">MQTT Topic</td>"
    "<td>: "; page += sonoffConfig.mqtt_topic; page += "</td>"
    "</tr>"
    "</table>"
    "<h3>Temperature Sensor:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">Present?</td>"
    "<td>: "; 
    page += (sonoffConfig.temp_present?"Yes":"No");
    page += "</td>"
    "</tr>";
    if (sonoffConfig.temp_present) {
      page += 
      "<tr>"
      "<td class=\"header\">Correction</td>"
      "<td>: "; page += sonoffConfig.temp_correction; page += "</td>"
      "</tr>"
      "<tr>"
      "<td class=\"header\">Interval</td>"
      "<td>: "; page += sonoffConfig.temp_interval; page += "sec.</td>"
      "</tr>";
    }
    page += "</table>";

  generatePage(page);
}

void handleConfiguration() {

  Serial << "Server: configuration" << endl;
  String page =
    "<form action=\"/save\"  method=\"post\">"
    "<h3>WiFI configuration:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">WiFi SSID<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"wifi_ssid\" length=32 value=\""; page += sonoffConfig.wifi_ssid; page += "\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">WiFi Password<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"wifi_password\" length=32 value=\""; page += sonoffConfig.wifi_password; page += "\" /></td>"
    "</tr>"
    "</table>"
    "<h3>MQTT Broker configuration:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">Host<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"mqtt_host\" length=32 value=\""; page += sonoffConfig.mqtt_host; page += "\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">Port<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"number\" name=\"mqtt_port\" length=5 value=\""; page += sonoffConfig.mqtt_port; page += "\"/></td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">User</td>"
    "<td>: <input type=\"text\" name=\"mqtt_user\" length=32 value=\""; page += sonoffConfig.mqtt_user; page += "\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">Password</td>"
    "<td>: <input type=\"text\" name=\"mqtt_password\" length=32 value=\""; page += sonoffConfig.mqtt_password; page += "\" /></td>"
    "</tr>"
    "<tr>"
    "<td class=\"header\">Topic<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"text\" name=\"mqtt_topic\" length=32 value=\""; page += sonoffConfig.mqtt_topic; page += "\" /></td>"
    "</tr>"
    "</table>"
    "<h3>Temperature sensor configuration:</h3>"
    "<table>"
    "<tr>"
    "<td class=\"header\">Is present?<sup class=\"red\">*</sup></td>"
    "<td>: <input type=\"checkbox\" name=\"temp_present\" length=1 value=\"1\"";
  if (sonoffConfig.temp_present) {
    page += " checked ";
  }

  page += "\" /></td>"
          "</tr>"
          "<tr>"
          "<td class=\"header\">Correction</td>"
          "<td>: <input type=\"text\" name=\"temp_correction\" length=5 value=\""; page += sonoffConfig.temp_correction; page += "\" /></td>"
          "</tr>"
          "<tr>"
          "<td class=\"header\">Interval (in sec.)</td>"
          "<td>: <input type=\"text\" name=\"temp_interval\" length=8 value=\""; page += sonoffConfig.temp_interval; page += "\" /></td>"
          "</tr>"
          "</table>"
          "<input class=\"submit\" type=\"submit\" />"
          "</form>";


  generatePage(page);
}

void handleNotFound() {
  Serial << "Server: page not found" << endl;

  String page = "Page Not Found\n\n";
  page += "URI: ";
  page += server.uri();
  page += "\nMethod: ";
  page += (server.method() == HTTP_GET) ? "GET" : "POST";
  page += "\nArguments: ";
  page += server.args();
  page += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    page += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  Serial << page << endl;
  generatePage(page);
}

void handleReboot() {
  Serial << "Server: rebooting device" << endl;
  String page =
    "<h4>Rebooting device in 3sec.</h4>";
  generatePage(page);
  toggleMode();
}

void handleSave() {
  LED.stopBlinking();
  LED.On();

  Serial << "Server: saving data" << endl;

  String _wifi_ssid = server.arg("wifi_ssid");
  String _wifi_password = server.arg("wifi_password");
  String _mqtt_host = server.arg("mqtt_host");
  String _mqtt_port = server.arg("mqtt_port");
  String _mqtt_user = server.arg("mqtt_user");
  String _mqtt_password = server.arg("mqtt_password");
  String _mqtt_topic = server.arg("mqtt_topic");

  String _temp_correction = server.arg("temp_correction");
  String _temp_interval = server.arg("temp_interval");

  String _temp_present = server.arg("temp_present");


  Serial << "TEMP=" << _temp_present << " ... length= " << _temp_present.length() << endl;

  bool saveing = false;

  if (_wifi_ssid.length() > 0) {
    memory.saveWiFiSSID(_wifi_ssid);
    saveing = true;
  }

  if (_wifi_password.length() > 0) {
    memory.saveWiFiPassword(_wifi_password);
    saveing = true;
  }

  if (_mqtt_host.length() > 0) {
    memory.saveMQTTHost(_mqtt_host);
    saveing = true;
  }

  if (_mqtt_port.length() > 0) {
    memory.saveMQTTPort(_mqtt_port.toInt());
    saveing = true;
  }

  if (_mqtt_user.length() > 0) {
    memory.saveMQTTUser(_mqtt_user);
    saveing = true;
  }

  if (_mqtt_password.length() > 0) {
    memory.saveMQTTPassword(_mqtt_password);
    saveing = true;
  }

  if (_mqtt_topic.length() > 0) {
    memory.saveMQTTTopic(_mqtt_topic);
    saveing = true;
  }

  if (_temp_present.length() > 0 ) {
    memory.saveTemperatureSensorPresent(1);
    saveing = true;
  } else {
    if (sonoffConfig.temp_present) {
      memory.saveTemperatureSensorPresent(0);
      saveing = true;
    }
  }

  if (_temp_correction.length() > 0 ) {
    memory.saveTemperatureCorrection(_temp_correction.toFloat());
    saveing = true;
  }

  if (_temp_interval.length() > 0) {
    memory.saveTemperatureInterval(_temp_interval.toInt());
    saveing = true;
  }

  if (saveing) {
    sonoffConfig = memory.getConfiguration();
  }

  String page =
    "<h3 style='margin-top:70px;'>Configuration has been successfully saved.</h3>"
    "<p style='margin-bottom:70px;'>Exit configuration to apply all changes</p>";

  generatePage(page);

  LED.startBlinking(0.1);

}

void handleReset() {
  Serial << "Server: resetting device" << endl;
  String page =
    "<h4>Resetting device in 6sec.</h4>";
  generatePage(page);
  resetDeviceMode();
}


String handleFlash() {
  Serial << "Server: firmware update" << endl;
  String page =
    "<h4>Resetting device in 6sec.</h4>";

  return page;
}

void generatePage(String &page) {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  page = PAGE_HEADER + page + PAGE_FOOTER;
  server.send(200, "text/html", page);
}

