
// uint8_t throttleFanspeed=PWM_THROTTLE; // ca 20 % speed


void publishDesSpeed(uint8_t speed){
  PWM_THROTTLE = speed; 
 
  // todo for test pupose - to write here ist not necessarily correct if standalone ESP866 code control loop of fan speed, 
  // and MQTT server is only responsible for desired values, limits etc
  //analogWrite(pwmGpio, speed);
  if (mqttClient.connected()){
    mqttClient.publish((MQTT_PUB_DES_PREFIX + MQTT_PUB_FANTHROTTLE_SUFFIX).c_str(), 0, true, String(speed, DEC).c_str());
    delay(10);
#if SERIAL_TRACE
    Serial.print("desired speed published: ");
    Serial.println(PWM_THROTTLE, HEX);
#endif
  } else {
    Serial.print("MQTT not up - desired speed not published: ");
    Serial.println(PWM_THROTTLE, HEX);
  }
}


void testDesiredFanspeed(const char* payload, const char* topic)
{
  String strComp((MQTT_PUB_DES_PREFIX + MQTT_PUB_FANTHROTTLE_SUFFIX).c_str());
  if (strComp.compareTo(topic)!=0)
  {
#if SERIAL_TRACE
    Serial.print (topic);
    Serial.print(" - testDesiredFanspeed - not mine expected:");
    Serial.println(MQTT_PUB_DES_PREFIX + MQTT_PUB_FANTHROTTLE_SUFFIX);
#endif    
    return;
  }
  unsigned long lRes = (unsigned)atol(payload);
#if MQTT_RECEIVE_ECHO  
  publishDesSpeed(lRes & 0xFF);
  delay(10);
#else
  PWM_THROTTLE=lRes&0xFF;
#endif  

}

