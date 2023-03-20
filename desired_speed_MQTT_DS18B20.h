
uint8_t throttleFanspeed=PWM_THROTTLE; // ca 20 % speed


void publishDesSpeed(uint8_t speed){
  throttleFanspeed = speed; 
 
  // todo for test pupose - to write here ist not necessarily correct if standalone ESP866 code control loop of fan speed, 
  // and MQTT server is only responsible for desired values, limits etc
  //analogWrite(pwmGpio, speed);
  mqttClient.publish(MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX, 0, true, String(speed, DEC).c_str());
  Serial.print("desired speed published: ");
  Serial.println(throttleFanspeed, HEX);
}


void testDesiredFanspeed(char* payload, char* topic)
{
  String strComp(MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX);
  if (strComp.compareTo(topic)!=0)
  {
    // Serial.print (topic);
    // Serial.print(" - testDesiredFanspeed - not mine expected:");
    // Serial.println(MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX);
    return;
  }
  unsigned long lRes = (unsigned)atol(payload);
  publishDesSpeed(lRes & 0xFF);
// echo message  ?
  // Serial.print ("Desired speed echoed: ");  
  // Serial.println (payload);
 // already echoed in publishDesSpeed() - removed: mqttClient.publish(topic, 1, true, payload);
}

