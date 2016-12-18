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


  String _wifi_ssid = server.arg("wifi_ssid");
  String _wifi_password = server.arg("wifi_password");
  String _mqtt_host = server.arg("mqtt_host");
  String _mqtt_port = server.arg("mqtt_port");
  String _mqtt_user = server.arg("mqtt_user");
  String _mqtt_password = server.arg("mqtt_password");

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
    memory.saveMQTTPort(_mqtt_port);
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

  if (saveing) {
    sonoffConfig = memory.getConfiguration();
  }
  
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
"</table>";

  generatePage(page);
}

void handleConfiguration() {
 
  Serial << "Server: configuration" << endl;
  String page = 
"<h3>WiFI configuration:</h3>"
"<form action=\"/\"  method=\"post\">"
"<table>"
    "<tr>"
        "<td class=\"header\">WiFi SSID<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"wifi_ssid\" length=32 value=\""; page += memory.getWiFiSSID(); page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">WiFi Password<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"wifi_password\" length=32 value=\""; page += memory.getWiFiPassword(); page += "\" /></td>"
    "</tr>"
"</table>"
"<h3>MQTT Broker configuration:</h3>"
"<table>"
    "<tr>"
        "<td class=\"header\">Host<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"mqtt_host\" length=32 value=\""; page += memory.getMQTTHost(); page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Port<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"number\" name=\"mqtt_port\" length=5 value=\""; page += memory.getMQTTPort(); page += "\"/></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">User</td>"
        "<td>: <input type=\"text\" name=\"mqtt_user\" length=32 value=\""; page += memory.getMQTTUser(); page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Password</td>"
        "<td>: <input type=\"text\" name=\"mqtt_password\" length=32 value=\""; page += memory.getMQTTPassword(); page += "\" /></td>"
    "</tr>"
"</table>"
"<input class=\"submit\" type=\"submit\" />"
"</form>";

  
  generatePage(page);
}

void handleUpdate() {
  Serial << "Server: configuration" << endl;
  String page = "Firmware update";
  generatePage(page);
}


void handleSave() {

  Serial << "Server: saveing data" << endl;
  
  String _wifi_ssid = server.arg("wifi_ssid");
  String _wifi_password = server.arg("wifi_password");
  String _mqtt_host = server.arg("mqtt_host");
  String _mqtt_port = server.arg("mqtt_port");
  String _mqtt_user = server.arg("mqtt_user");
  String _mqtt_password = server.arg("mqtt_password");

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
    memory.saveMQTTPort(_mqtt_port);
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
  String page =
  "<h4>Information saved</h4>"; 

  generatePage(page);
  
}

void handleNotFound(){
  Serial << "Server: page not found" << endl;
  String page = "Page not found";
  generatePage(page);
}


void generatePage(String &page) {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  page = PAGE_HEADER + page + PAGE_FOOTER;
  server.send(200, "text/html", page);
  blinkLED();
}


void runConfigurationMode() {  
  Serial << endl << "Starting configuration mode" << endl;
  Serial << " - launching access point" << endl;
  
  IPAddress apIP(192, 168, 5, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(hostName);
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "www.example.com", apIP);
  
  Serial << " - launching web server" << endl;
  server.on("/", handleRoot);
  server.on("/configure", handleConfiguration);
  server.on("/update", handleUpdate);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial << " - ready " << endl << endl;   
}

