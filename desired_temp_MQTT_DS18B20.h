





void publishDesTemp(float partemp){
  desired_temp = partemp; 
  char buffer[10];
  sprintf(buffer,"%.2f", partemp); 
  mqttClient.publish(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX, 0, true, buffer);
  Serial.print("desired temp published: ");
  Serial.println(buffer);
}


void publishTempHyst(float partemp){
 temp_hyst = partemp;
  char buffer[10];
  sprintf(buffer,"%.2f", partemp); 
  mqttClient.publish(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX, 0, true, buffer);
  Serial.print("desired temp published: ");
  Serial.println(buffer);

}


void testDesiredTemperature(char* payload, char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
    Serial.print (topic);
    Serial.print(" - testDesiredTemperature - not mine - ");
    Serial.println(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX);
    return;
  }
  desired_temp = atof(payload);

// echo message  ?
  Serial.print ("Desired temp echoed: ");  
  Serial.println(payload);
  mqttClient.publish(topic, 1, true, payload);
}

void testDesiredTempHyst(char* payload, char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
    Serial.print (topic);
    Serial.print(" - testDesiredTempHyst - not mine - ");
    Serial.println(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX);
    return;
  }
  temp_hyst = atof(payload);

// echo message  ?
  Serial.print ("Desired temphyst echoed: ");  
  Serial.println(payload);
  mqttClient.publish(topic, 1, true, payload);
}


