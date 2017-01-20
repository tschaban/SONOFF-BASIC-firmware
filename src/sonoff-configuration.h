/*
 Sonoff: firmware
 More info: https://github.com/tschaban/SONOFF-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_configuration_h
#define _sonoff_configuration_h


#define MODE_SWITCH 0
#define MODE_CONFIGURATION 1
#define MODE_ACCESSPOINT 2

#define TEMP_SENSOR 14


/* Configuration parameters */
#define   CONNECTION_WAIT_TIME 500

struct DEFAULTS {
  char          version[8] = "0.3.3";
  unsigned int  mqtt_port = 1883;
  float         temp_correction = 0;
  unsigned int  temp_interval = 600;
  boolean       temp_present = false;     
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
"        table {margin: 2px 15px 15px 15px;}"
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
"<footer>(c)2017 Sonoff Configuration Panel, <a href=\"https://github.com/tschaban/SONOFF-firmware\" target=\"_blank\">GitHub</a></footer>"
"</div>"
"</div>"
"</body>"
"</html>";

#endif
