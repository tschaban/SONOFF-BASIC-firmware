/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#define RELAY 12
#define LED 13
#define BUTTON 0
#define TEMP_SENSOR 14

/* Configuration parameters */
#define   CONNECTION_WAIT_TIME 500

struct DEFAULTS {
  char          version[8] = "0.2.15";
  unsigned int  mqtt_port = 1883;
  float         temp_correction = 0;
  unsigned int  temp_interval = 600;     
 };


struct SONOFFCONFIG {
  char          version[8];

  char          id[6] = {0};
  char          host_name[13] = {0};
  
  char          update_url[100];
  unsigned int  mode;
  char          wifi_ssid[32];
  char          wifi_password[32];

  char          mqtt_host[32];
  unsigned int  mqtt_port;
  char          mqtt_user[32];
  char          mqtt_password[32];
  char          mqtt_topic[32];

  float         temp_correction;
  unsigned int  temp_interval;

};


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
"        .container{max-width: 550px; margin: 10px auto; height: auto}"
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
"        <li>| <a href=\"/\">Start</a> | </li>"
"        <li><a href=\"/configure\">Configuration</a> | </li>"
"        <li><a href=\"/update\">Firmware update</a> | </li>"
"        <li><a href=\"/reset\">Reset to factory settings</a> |</li>"
"        <li><a href=\"/reboot\">Exit</a> | </li>"
"    </ul>"
"    <hr />"
"    <div class=\"content\">";

const String PAGE_FOOTER =
"<hr />"
"<footer>(c) 2017 <a href=\"https://github.com/tschaban/SONOFF-firmware\" target=\"_blank\">GitHub</a></footer>"
"</div>"
"</div>"
"</body>"
"</html>";
