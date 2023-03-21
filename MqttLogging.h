void MQTTLog(const char* logmsg){
    
    Serial.print("mqttlog "); 
    Serial.println(logmsg);
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DEV_PREFIX "/logmsg", 1, true, logmsg);                            
}
