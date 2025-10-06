void DefaultCallback(const char* value) {
  Serial.println("BINDING-Default callback: ");
  // Serial.println(value);
}

void setMQTTBindings() {
   tempHystNode.SetBinding((MQTT_PUB_DES_PREFIX + MQTT_PUB_TEMPHYST_SUFFIX).c_str(),DefaultCallback);
}