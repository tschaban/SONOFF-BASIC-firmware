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
#define   ID ESP.getChipId()        // Device ID
#define   CONNECTION_WAIT_TIME 500


/* Variables, temporarly here */

char  mqttTopic[50];  // it stories topic which is MQTT_TOPIC/ID/

struct DEFAULTS {
  unsigned int mqtt_port = 1883;
  char          mqtt_topic[32] = "/sonoff/";
  float         temp_correction = 0;
  unsigned int  temp_interval = 600;     
 } sonoffDefault;


struct SONOFFCONFIG {
  unsigned long version;
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
