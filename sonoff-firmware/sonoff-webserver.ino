/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

const String PAGE_HEADER =
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
"    <meta charset=\"UTF-8\">"
"    <meta name=\"viewport\" content=\"user-scalable = yes\">"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"    <title>Sonoff Configuration</title>"
"    <style>"
"        body {font-family: Tahoma;font-size: 14px}"
"        a {color: #1E90FF;text-decoration: none;}"
"        table {margin: 2px 0 15px 0;}"
"        td {}"
"        h1,h3 {color: #AA5F39;margin: 0px;}"
"        hr {height: 1px;border: 0 none;background-color: #eee;}"
"        ul {margin: 0;  padding: 0; list-style-type: none;}"
"        li {display: inline; padding-right: 5px;}"
"        footer {color: #aaaaaa; font-size: small;}"
"        .container{width: 350px; margin: 10px auto; height: auto}"
"        .content{margin: 5px 5px}"
"        .header {font-weight: bold;}"
"        .submit {margin: 5px 0; padding: 2px 10px; }"
"        .red {color: #FF0000;}"
"    </style>"
"</head>"
"<body>"
"<div class=\"container\">"
"    <h1>Sonoff</h1>"
"    <hr />"
"    <ul>"
"        <li><a href=\"/\">Home</a></li>"
"        <li><a href=\"/configure\">Configuration</a></li>"
"        <li><a href=\"/update\">Firmware</a></li>"
"        <li><a href=\"/reboot\">Reboot</a></li>"
"        <li><a href=\"/reset\">Reset to defaults</a></li>"
"    </ul>"
"    <hr />"
"    <div class=\"content\">";

const String PAGE_FOOTER =
"<hr />"
"<footer>Version: 0.1 beta</footer>"
"</div>"
"</div>"
"</body>"
"</html>";
          
void handleRoot() {
  Serial << "Server: root requested" << endl;
  
  String page =
"<h3>Device:</h3>"   
"<table>"
"<tr>"
"<td class=\"header\">ID</td>"
"<td>: "; page += ID; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">Name</td>"
"<td>: "; page += hostName; page += "</td>"
"</tr>" 
"</table>"
"<h3>Configuration:</h3>"   
"<table>"      
"<tr>"
"<td class=\"header\">WiFi SSID</td>"
"<td>: "; page += sonoffConfig.wifi_ssid; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">WiFi Password</td>"
"<td>: "; page += sonoffConfig.wifi_password; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">MQTT Host</td>"
"<td>: "; page += sonoffConfig.mqtt_host; page += ":";  page += sonoffConfig.mqtt_port; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">MQTT User</td>"
"<td>: "; page += sonoffConfig.mqtt_user; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">MQTT Password</td>"
"<td>: "; page += sonoffConfig.mqtt_password; page += "</td>"
"</tr>"         
"</table>"
"<h3>Temp.Sensor:</h3>"   
"<table>"
"<tr>"
"<td class=\"header\">Correction</td>"
"<td>: "; page += sonoffConfig.temp_correction; page += "</td>"
"</tr>"
"<tr>"
"<td class=\"header\">Interval</td>"
"<td>: "; page += sonoffConfig.temp_interval; page += "sec.</td>"
"</tr>" 
"</table>";

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
        "<td class=\"header\">Topic</td>"
        "<td>: <input type=\"text\" name=\"mqtt_topic\" length=32 value=\""; page += sonoffConfig.mqtt_topic; page += "\" /></td>"
    "</tr>"    
"</table>"
"<h3>Temperature sensor configuration:</h3>"
"<table>"
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

void handleUpdate() {
  Serial << "Server: update mode" << endl;
  String page =
  "<h4>Going to update mode in 3sec.</h4>"; 
  generatePage(page);  
  flashMode();
  ESP.restart();  
}



void handleNotFound(){
  Serial << "Server: page not found" << endl;
  String page = "Page not found";
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
  stopBlinkLEDInLoop();
  LEDOn();

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
  "<h3>Saveing data</h3>"
  "- completed";
     
  generatePage(page);

  blinkLEDInLoop(1);
  
}

void handleReset() {
  Serial << "Server: resetting device" << endl;
  String page =
  "<h4>Resetting device in 6sec.</h4>"; 
  generatePage(page);
  resetDeviceMode();
}

void generatePage(String &page) {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  page = PAGE_HEADER + page + PAGE_FOOTER;
  server.send(200, "text/html", page);
}

