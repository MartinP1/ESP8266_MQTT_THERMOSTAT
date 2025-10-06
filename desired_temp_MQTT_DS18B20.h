





void publishDesTemp(float partemp){
  desired_temp = partemp; 
  char buffer[10];
  sprintf(buffer,"%.2f", partemp); 
  if (mqttClient.connected())
    mqttClient.publish((MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMP_SUFFIX).c_str(), 0, true, buffer);
  delay(10);
#if SERIAL_TRACE  
  Serial.print("desired temp published: ");
  Serial.println(buffer);
#endif  
}


void publishTempHyst(float partemp){
 temp_hyst = partemp;
  char buffer[10];
  sprintf(buffer,"%.2f", partemp); 
  if (mqttClient.connected())
    mqttClient.publish((MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMPHYST_SUFFIX).c_str(), 0, true, buffer);
  delay(10);  
#if SERIAL_TRACE
  Serial.print("desired temp hyst published: ");
  Serial.println(buffer);
#endif
}


void testDesiredTemperature(const char* payload, const char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMP_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
        
#if SERIAL_TRACE
    Serial.print (topic);
    Serial.print(" - testDesiredTemperature - not mine - ");
    Serial.println(MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMP_SUFFIX);
#endif
    return;
  }
  desired_temp = atof(payload);

// echo message  ?
#if MQTT_RECEIVE_ECHO    
#if SERIAL_TRACE
  Serial.print ("Desired temp echoed: ");  
  Serial.println(payload);
#endif 
  if (mqttClient.connected())
    mqttClient.publish(topic, 1, true, payload);
  delay(10);
#endif  
}

void testDesiredTempHyst(const char* payload, const char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMPHYST_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
  #if SERIAL_TRACE
    Serial.print (topic);
    Serial.print(" - testDesiredTempHyst - not mine - ");
    Serial.println(MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMPHYST_SUFFIX);
  #endif
    return;
  }
  temp_hyst = atof(payload);
 
// echo message  ?
#if MQTT_RECEIVE_ECHO    
#if SERIAL_TRACE
  Serial.print ("Desired temphyst echoed: ");  
  Serial.println(payload);
#endif    
  if (mqttClient.connected())
    mqttClient.publish(topic, 1, true, payload);
  delay(10);
#endif  
}


