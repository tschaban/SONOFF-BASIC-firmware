float tempCorrection = sonoffConfig.temp_correction;
float previousTemperature = 0;


/* Pubish temperature to MQTT broker */
void publishTemperature() {
  char  temperatureString[6];
  char  mqttString[50];
  float temperature = getTemperature();
  dtostrf(temperature, 2, 2, temperatureString);
  if (previousTemperature!=temperature) {
    Serial << " - publishing: " << temperatureString << endl;
    sprintf(mqttString,"%stemperature", sonoffConfig.mqtt_topic);
    client.publish(mqttString, temperatureString);
    previousTemperature=temperature;
  }
}


/* Set how often sensor should be read */
void setSensorReadInterval(int interval) {
  temperatureTimer.detach();
  temperatureTimer.attach(interval, publishTemperature);
}


/* Get temperature */
float getTemperature() {
  float temperature;
  Serial << endl << "Requesting temperature" << endl;
  do {
    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
  } while (temperature == 85.0 || temperature == (-127.0));
  Serial << endl << " - temperature: " << temperature << endl;
  return temperature + tempCorrection;
}
