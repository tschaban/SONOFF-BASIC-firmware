/*
 SONOFF BASIC: firmware
 More info: https://github.com/tschaban/SONOFF-BASIC-firmware
 LICENCE: http://opensource.org/licenses/MIT
 2016-10-27 tschaban https://github.com/tschaban
*/

#ifndef _sonoff_configuration_h
#define _sonoff_configuration_h

/* Configuration parameters */
#define   MODE_SWITCH 0
#define   MODE_CONFIGURATION 1
#define   MODE_ACCESSPOINT 2

/* Values for EEPROM Parameter Relay state after crash */
#define   DEFAULT_RELAY_ON 1
#define   DEFAULT_RELAY_OFF 2
#define   DEFAULT_RELAY_LAST_KNOWN 3
#define   DEFAULT_RELAY_SERVER 4

#define   RELAY_OFF 0
#define   RELAY_ON 1

#define   SWITCH_SENSITIVENESS_HIGH 0
#define   SWITCH_SENSITIVENESS_NORMAL 1
#define   SWITCH_SENSITIVENESS_LOW 2
#define   SWITCH_SENSITIVENESS_HIGH_VALUE 0
#define   SWITCH_SENSITIVENESS_NORMAL_VALUE 3
#define   SWITCH_SENSITIVENESS_LOW_VALUE 10 

#define   INTERFACE_NONE 0
#define   INTERFACE_MQTT 1
#define   INTERFACE_HTTP 2

#define   GPIO_1 1
#define   GPIO_3 3
#define   GPIO_14 14


struct DEFAULTS {
  char          version[8] = "0.8.1";
  float         temp_correction = 0;
  char          language[3] = "en";
  unsigned int  mqtt_port = 1883;
  unsigned int  temp_interval = 600;
  boolean       temp_present = false; 
  uint8_t       relay_state_after_power_restored = DEFAULT_RELAY_LAST_KNOWN;
  uint8_t       relay_state_after_connection_restored = DEFAULT_RELAY_SERVER;
  boolean       switch_present = false;
  uint8_t       switch_sensitiveness = SWITCH_SENSITIVENESS_NORMAL;
  uint8_t       switch_gpio = GPIO_14;
  uint8_t       number_connection_attempts = 20;
  uint8_t       duration_between_connection_attempts = 1; // seconds
  uint8_t       duration_between_next_connection_attempts_series = 1; // minutes
          
 };


struct SONOFFCONFIG {
  char          version[8];
  char          language[3];
  char          id[6] = {0};
  char          device_name[32] = {0};
  
  char          update_url[100];
  uint8_t       mode;
  uint8_t       interface;
  
  char          wifi_ssid[32];
  char          wifi_password[32];

  char          mqtt_host[32];
  unsigned int  mqtt_port;
  char          mqtt_user[32];
  char          mqtt_password[32];
  char          mqtt_topic[32];

  float         ds18b20_correction;
  unsigned int  ds18b20_interval;
  boolean       ds18b20_present;
  boolean       switch_present;
  uint8_t       switch_sensitiveness;
  uint8_t       switch_gpio;

  uint8_t       number_connection_attempts;
  uint8_t       duration_between_connection_attempts;
  uint8_t       duration_between_next_connection_attempts_series;

  boolean       sleep_mode = false;

  boolean       debugger;
};


#endif

