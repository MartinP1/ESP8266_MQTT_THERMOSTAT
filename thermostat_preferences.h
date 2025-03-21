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

Preferences prefs;

void getPreferences() {
  prefs.begin("mqtt_thermostat", true);
  if (prefs.isKey("MqttName")){
    String strTmp = prefs.getString("MqttName");
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
  if (prefs.isKey("Debug")){
    uiDebug = prefs.getUChar("Debug");
    Serial.print("INFO: Debug ");
    Serial.println(uiDebug);
    }
  else Serial.println("WARN: No Debug setting found in preferences");
  
  prefs.end();
  
}


void testPreferences(char* payload, const char* topic){
  size_t siz;
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
      prefs.begin("mqtt_thermostat");
      siz = prefs.putString( "MqttName", payload);
      prefs.end();
      Serial.print("Prefs.MqttName written ");
      Serial.print(siz);
      Serial.print(" bytes - ");
      Serial.println(payload);
    }
    if (mqttClient.connected()) {
      (topic, 1, true, payload);
    }
    return;
 
  }
  strComp=MQTT_PUB_DEV_PREFIX +"/Preferences/Debug";
  if (strComp.compareTo(topic)==0) {
    if (mqttClient.connected())
      mqttClient.publish(topic, 1, true, payload);    // do it here due to several returns
    uint8_t uiDebuglocal = (uint8_t)atol(payload);
    // echo message  ?
    if (uiDebuglocal != uiDebug) uiDebug = uiDebuglocal;
    else return;

    Serial.print ("Preferences Debug: ");  
    Serial.println(uiDebug);
    prefs.begin("mqtt_thermostat");
    siz = prefs.putUChar( "Debug", uiDebug);
    prefs.end();
    return;
  }


// no preferences topic for me
  #if SERIAL_TRACE
    Serial.print (topic);
    Serial.println(" preferences - not mine - ");
  #endif
    return;
  }
