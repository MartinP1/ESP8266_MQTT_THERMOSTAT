  char log_buffer[120];


void MQTTLog(const char* logmsg){
    
    Serial.print("mqttlog "); 
    Serial.println(logmsg);
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DEV_PREFIX "/logmsg", 1, true, logmsg); 
    delay(10);                           
}


void MQTTLogPrintf (const char* format, ...){
  va_list args;
  va_start (args, format);
  vsnprintf (log_buffer,119,format, args);
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DEV_PREFIX "/logmsg", 1, true, log_buffer);    
  delay(10);                        
  va_end (args);
}