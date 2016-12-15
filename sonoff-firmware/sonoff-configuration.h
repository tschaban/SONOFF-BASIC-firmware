#define RELAY 12
#define LED 13
#define BUTTON 0
#define TEMP_SENSOR 14

/* Configuration parameters */
#define    ID            ESP.getChipId()    // Device ID

#define  WIFI_SSID       "<SSID>"               // WiFi Name
#define  WIFI_PASSWORD   "<password>"           // WiFi Password

#define  MQTT_HOST       "<host>"               // MQTT Broker Host
#define  MQTT_PORT       1883                   // MQTT Port
#define  MQTT_USER       "<user>"               // MQTT User
#define  MQTT_PASSWORD   "<password>"           // MQTT Password


#define  MQTT_TOPIC      "/sonoff/"             // MQTT Topic

#define  TEMP_CORRECTION  0                     // Temperature correction
#define  TEMP_INTERVAL    600                   // How often temperature sensor should be read

#define   CONNECTION_WAIT_TIME 500

/* Variables, temporarly here */

char  mqttTopic[50];  // it stories topic which is MQTT_TOPIC/ID/
unsigned long pressedCount = 0;
float tempCorrection = TEMP_CORRECTION;
float previousTemperature = 0;


struct SONOFFCONFIG {
  unsigned long version;
  char          update_url[100];

  char          wifi_ssid[32];
  char          wifi_password[64];

  char          mqtt_host[32];
  uint16_t      mqtt_port;
  char          mqtt_user[32];
  char          mqtt_password[32];
  char          mqtt_topic[32];

} sonoffConfig;
