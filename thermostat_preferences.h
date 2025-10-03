/**
 *  thermostat_preferences.h
 * 
 * use preferences library to restore/store preferences
 * that shall not be hard coded
 * 
 * namespace mqtt_thermostat
 * defined preferences
 * MqttName string
 * debug unsigned char (bit 1 defined, pwm passthrough)
 * MP 16-JUN-2024
 * 
 * 
*/



#define WRITE_TO_NV 1

/*********************************************************

Here is our list of Preferences keys

 Attention, take in Mind that preferences keys/names are
 limited to 15 characters
*********************************************************/
const char *OverrideWindowSensor_key="OvrWinSens";
const char *MqttName_key="MqttName";
const char *Debug_key="Debug";

Preferences prefs;

void getPreferences() {
  prefs.begin("mqtt_thermostat", true);
  if (prefs.isKey(MqttName_key)){
    String strTmp = prefs.getString(MqttName_key);
    strTmp.trim();
    if (strTmp.length()>2) {

      MQTT_PUB_DEV_PREFIX = prefs.getString("MqttName");
      Serial.print("INFO: MqttName ");
      Serial.println(MQTT_PUB_DEV_PREFIX);
    } else {
      Serial.print("WARN: MqttName found in preferences is too short ");
      Serial.println(strTmp.c_str());
    }
  }
  else Serial.println("WARN: No MqttName found in preferences");
  if (prefs.isKey(Debug_key)){
    uiDebug = prefs.getUChar(Debug_key);
    Serial.print("INFO: Debug ");
    Serial.println(uiDebug);
  }
  else Serial.println("WARN: No Debug setting found in preferences");
  if (prefs.isKey(OverrideWindowSensor_key)){
     uiOverrideWindowSensor = prefs.getUChar(OverrideWindowSensor_key);
    Serial.print("INFO: uiOverrideWindowSensor ");
    Serial.println(uiOverrideWindowSensor);
  }
  else Serial.println("WARN: No OverrideWindowsSensor setting found in preferences");
  prefs.end();
  
}


void testPreferences(char* payload, const char* topic){
  size_t siz;
  // Serial.printf("\n testPreferences - Free Stack Space: %d", uxTaskGetStackHighWaterMark(NULL));
  String strComp((MQTT_PUB_DEV_PREFIX +"/Preferences/MqttName").c_str());
  if (strComp.compareTo(topic)==0)
  {
  // echo message  ?
#if SERIAL_TRACE
    Serial.print ("Preferences MqttName: ");  
    Serial.println(payload);
#endif
    if (MQTT_PUB_DEV_PREFIX.equals(topic)) {
      Serial.print("Prefs.MqttName is not changed ");
      Serial.println(payload);
    } else {

#if WRITE_TO_NV
      prefs.begin("mqtt_thermostat");
      siz = prefs.putString( MqttName_key, payload);
      prefs.end();
#endif      
      Serial.print("Prefs.MqttName written ");
      Serial.print(siz);
      Serial.print(" bytes - ");
      Serial.println(payload);
    }
#if MQTT_RECEIVE_ECHO    
    if (mqttClient.connected()) {
      mqttClient.publish(topic, 1, true, payload);
    }
#endif      
    return;
 
  }
  strComp=MQTT_PUB_DEV_PREFIX +"/Preferences/Debug";
  if (strComp.compareTo(topic)==0) {
#if MQTT_RECEIVE_ECHO    
    if (mqttClient.connected())
      mqttClient.publish(topic, 1, true, payload);    // do it here due to several returns
#endif    
    uint8_t uiDebuglocal = (uint8_t)atol(payload);
    // echo message  ?
    if (uiDebuglocal != uiDebug) uiDebug = uiDebuglocal;
    else return;

    Serial.print ("Preferences Debug: ");  
    Serial.println(uiDebug);
#if WRITE_TO_NV
    prefs.begin("mqtt_thermostat");
    siz = prefs.putUChar( Debug_key, uiDebug);
    prefs.end();
#endif
    return;
  }
  strComp=MQTT_PUB_DEV_PREFIX +"/Preferences/OverrideWindowsSensor";
  if (strComp.compareTo(topic)==0) {
  #if 1
  #if MQTT_RECEIVE_ECHO    
    if (mqttClient.connected())
      mqttClient.publish(topic, 1, true, payload);    // do it here due to several returns
  #endif    
    uint8_t uiOverrideWindowSensorlocal = (uint8_t)atol(payload);
    // echo message  ?
    if (uiOverrideWindowSensorlocal != uiOverrideWindowSensor) 
       uiOverrideWindowSensor = uiOverrideWindowSensorlocal;
    else return;

    Serial.print ("Preferences OverrideWindowSensor");  
    Serial.println(uiOverrideWindowSensor);
#if WRITE_TO_NV
    prefs.begin("mqtt_thermostat");
    siz = prefs.putUChar( OverrideWindowSensor_key, uiOverrideWindowSensor);
    prefs.end();
    Serial.print(", written ");
    Serial.print(siz);
    Serial.print(" bytes - value: ");
    Serial.println(uiOverrideWindowSensor);
#else    
    Serial.print(", not written ");
    Serial.print("-  value: ");
    Serial.println(uiOverrideWindowSensor);
#endif
#endif
    return;
  }


// no preferences topic for me
#if SERIAL_TRACE
  Serial.print (topic);
  Serial.println(" preferences - not mine - ");
#endif
  return;
}
