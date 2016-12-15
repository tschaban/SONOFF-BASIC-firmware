const String PAGE_HEADER =
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
    "<meta charset=\"UTF-8\">"
    "<title>Sonoff Configuration</title>"
    "<style>"
        "body{font-family:tahoma;font-size: 16px;}"
        "footer{font-size: 12px;color:#aaaaaa;}"
        "td{font-size: 14px;}"
        "h2{color:#AA5F39;margin: 0px;}"
        "h4{color:#123652;}"
        "hr{height:1px;border: 0 none;background-color: #eee;}"
        ".header{font-weight: bold;}"
        ".submit{margin: 5px 0; padding: 2px 10px;}"
        ".red{color:#FF0000;}"
    "</style>"
"</head>"
"<body>"
"<h2>Sonoff switch</h2>"
"<hr />";

const String PAGE_FOOTER =
"<hr />"
"<footer>Follow on <a href=\"https://github.com/tschaban/SONOFF-firmware/\" target=\"_blank\">GitHub</a></footer>"
"</body>"
"</html>";
        
          
void handleRoot() {
  Serial << "Server: root requested" << endl;
  String page = 
"<table>"
    "<tr>"
        "<td class=\"header\">Device ID</td>"
        "<td>: "; page += ID; page += "</td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Connected to WiFI</td>"
        "<td>: "; page += WIFI_SSID; page += "</td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Connected to MQTT</td>"
        "<td>: "; page += MQTT_HOST; page += "</td>"
    "</tr>"
"</table>"
"<h4>Options:</h4>"
"<ul>"
    "<li><a href=\"/configure\">Change configuration</a></li>"
    "<li><a href=\"/update\">Update firmware</a></li>"
"</ul>";

  generatePage(page);
}

void handleConfiguration() {
  String _wifi_ssid = server.arg("wifi_ssid");
  String _wifi_password = server.arg("wifi_password");

  if (_wifi_ssid.length() > 0 && _wifi_password.length() > 0) {  
    memory.saveWiFiSSID(_wifi_ssid);
  }

  if (_wifi_password.length() > 0) {  
    memory.saveWiFiPassword(_wifi_password);
  }
  
  Serial << "Server: configuration" << endl;
  String page = 
"<h4>WiFI configuration:</h4>"
"<form action=\"/configure\"  method=\"post\">"
"<table>"
    "<tr>"
        "<td class=\"header\">WiFi SSID<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"wifi_ssid\" length=32 value=\""; page += sonoffConfig.wifi_ssid; page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">WiFi Password<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"wifi_password\" length=64 value=\""; page += sonoffConfig.wifi_password; page += "\" /></td>"
    "</tr>"
"</table>"
"<h4>MQTT Broker configuration:</h4>"
"<table>"
    "<tr>"
        "<td class=\"header\">Host<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"text\" name=\"mqtt_host\" length=32 value=\""; page += MQTT_HOST; page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Port<sup class=\"red\">*</sup></td>"
        "<td>: <input type=\"number\" name=\"mqtt_port\" length=5 value=\""; page += MQTT_PORT; page += "\"/></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">User</td>"
        "<td>: <input type=\"text\" name=\"mqtt_user\" length=32 value=\""; page += MQTT_USER; page += "\" /></td>"
    "</tr>"
    "<tr>"
        "<td class=\"header\">Password</td>"
        "<td>: <input type=\"text\" name=\"mqtt_password\" length=32 value=\""; page += MQTT_PASSWORD; page += "\" /></td>"
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
