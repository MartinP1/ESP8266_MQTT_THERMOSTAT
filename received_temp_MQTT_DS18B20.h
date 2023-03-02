

void getTemperatures() {
   sensors.requestTemperatures(); 
    // Temperature in Celsius degrees
    for (int i=0; i<numberOfDevices && i<3; i++)
    {
      float temp = sensors.getTempCByIndex(i);
      // Temperature in Fahrenheit degrees
      //temp = sensors.getTempFByIndex(0);
    
      // Publish an MQTT message on topic esp/ds18b20/temperature
      String Topic(MQTT_PUB_TEMP_PREFIX);
      Topic += TempsensRole[i];
      Topic += String(MQTT_PUB_TEMP_SUFFIX);
      uint16_t packetIdPub1 = mqttClient.publish(Topic.c_str(), 1, true, String(temp).c_str());                            
      Serial.printf("Pubng on topic %s at QoS 1, packetId: %i ", Topic.c_str(), packetIdPub1);
      Serial.printf("Msg: %.2f \n", temp);
    }
}
