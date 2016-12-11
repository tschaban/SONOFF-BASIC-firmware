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
