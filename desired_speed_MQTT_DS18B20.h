
uint8_t pwmSet=255; // maxed out


void publishDesSpeed(uint8_t speed){
  pwmSet = speed; 
  // todo for test pupose - to write here ist not necessarily correct if standalone ESP866 code control loop of fan speed, 
  // and MQTT server is only responsible for desired values, limits etc
  analogWrite(pwmGpio, speed);
  mqttClient.publish(MQTT_PUB_DES_PREFIX MQTT_PUB_FANMAX_SUFFIX, 0, true, String(speed, DEC).c_str());
}


void testDesiredFanspeed(char* payload, char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX MQTT_PUB_FANMAX_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
    Serial.print (topic);
    Serial.print(" - testDesiredFanspeed - not mine expected:");
    Serial.println(MQTT_PUB_DES_PREFIX MQTT_PUB_FANMAX_SUFFIX);
    return;
  }
  unsigned long lRes = (unsigned)atol(payload);
  publishDesSpeed(lRes & 0xFF);
// echo message  ?
  Serial.print ("Desired temp echoed: ");  
  Serial.println ( topic);


 // already echoed in publishDesSpeed() - removed: mqttClient.publish(topic, 1, true, payload);
}

