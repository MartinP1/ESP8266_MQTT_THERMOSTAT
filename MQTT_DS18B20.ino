
#include "./GLOBAL_VARS_MQTT_DS18B20.h"
#include "unspecialized_mqtt_doings.h"
#include "MqttLogging.h"
#include "desired_temp_MQTT_DS18B20.h"
#include "desired_speed_MQTT_DS18B20.h"
#include "received_temp_MQTT_DS18B20.h"
#include "fan_pwm_MQTT_DS18B20.h"
// #include "ventile_control_MQTT_DS18B20.h"
// #include "window_contact_MQTT_DS18B20.h"
#include "do_calculations_MQTT_DS18B20.h"


#if !defined(ARDUINO_D1_MINI32) && !defined(ARDUINO_LOLIN_S2_MINI) && !defined(ARDUINO_ESP8266_WEMOS_D1MINI)
#error "unsupported board type"
#error ARDUINO_BOARD
#else
#pragma message ARDUINO_BOARD
#endif

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
      Serial.print(strTmp.c_str());
    }
  }
  else Serial.println("WARN: No MqttName found in preferences");
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
    return;
   // mqttClient.publish(topic, 1, true, payload);
  // write to preferences 
    // todo: further preferences topics...

  }


// no preferences topic for me
  #if SERIAL_TRACE
    Serial.print (topic);
    Serial.println(" preferences - not mine - ");
  #endif
    return;
  }

void onMqttConnect(bool sessionPresent) {
  Serial.print("Connected to MQTT - IP=");
  Serial.print(WiFi.localIP());
  Serial.print("- RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.print(" dB - Session present: ");
  Serial.println(sessionPresent);
  // todo not necessarily QOS 2, 1 or 0 are also ok... 
  uint16_t packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX).c_str(), 2);
  Serial.print("Subscribing desired temp at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  // publishDesTemp(desired_temp); must be created by mqtt broker
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX).c_str(), 2);
  Serial.print("Subscribing desired hyst at QoS 2, packetId: ");
  // publishTempHyst(temp_hyst); must be created by mqtt broker
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX).c_str(), 2);
  Serial.print("Subscribing max fan speed at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DEV_PREFIX +"/Preferences/MqttName").c_str(), 2);
  Serial.print("Subscribing desired device name, packetId: ");
  Serial.println(packetIdSub);
  publishDesSpeed(throttleFanspeed);
  
  MQTTLogPrintf("Thermostat started %d Thermosensors found", numberOfDevices);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  delay(10);
  return;
  }

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  // Serial.println("Subscribe acknowledged.");
  // Serial.print("  packetId: ");
  // Serial.println(packetId);
  // Serial.print("  qos: ");
  // Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  // Serial.println("Unsubscribe acknowledged.");
  // Serial.print("  packetSerial Id: ");
  // Serial.println(packetId);
}

void onMqttPublish(uint16_t packetId) {
  // Serial.print("Publish acknowledged.");
  // Serial.print("  packetId: ");
  // Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
#if 0  
  Serial.println(" Publish received.");// 
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
   Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
  Serial.print(" payload: ");
  for (int i=0; i<len; i++) {// 
    Serial.print(payload[i]);
    /home/martin/Arduino/MQTT/MQTT_DS18B20
  }  
  Serial.println(" OK");
#endif

  char buffer[30];
  buffer[29]=buffer[len]='\0';
  for (int i=0;i<len && i<29;i++){
    buffer [i] = payload[i];   
  }

  /// Distinguish path
  
  testDesiredTemperature(buffer, topic);  
  testDesiredTempHyst(buffer, topic);
  testDesiredFanspeed(buffer,topic);
  testPreferences(buffer, topic);
}

void setup() {
  sensors.begin();
  Serial.begin(115200);
  delay(4000);
  Serial.println("*** SETUP ***");
  getPreferences();
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  // ledcSetup(0, 8000, 8); // pwm#, freq, resolution(bits)
  // ledcAttachPin(pwmGpio, 0);
  ledcAttach(pwmGpio, 8000, 8);
#endif
#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  pinMode(pwmGpio, OUTPUT);
  analogWriteFreq(8000);
  analogWrite(pwmGpio,255); // raw write off!
#endif  
  pinMode(valveGpio, OUTPUT);
  pinMode(windowContact, INPUT); 
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  WiFi.onEvent(onWifiConnect,WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(onWifiDisconnect,WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
#else  
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
#endif

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
   mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  
  // mqttClient.setKeepAlive(30);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USE92:de:dc:42:f3:f8R", "REPLACE_WITH_YOUR_PASSWORD");
  connectToWifi();
  initTemperatureSensors();
  previousMillis = millis(); // avoid initial overrun?
}



// unsigned short usWifiDown=12;
void loop() {
  unsigned long currentMillis = millis();
  
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New temperature readings
    getTemperatures();

    runTempControl();
    wifiStatus();
#if 0
// does not help ...
    if (WiFi.isConnected() && mqttClient.connected())
    {
      usWifiDown=12;  // 2 Minuten
    }
    else
    {
      usWifiDown--;
      Serial.print(" Wifi Error count down ");
      Serial.println(usWifiDown);

      if (usWifiDown==0) // 2 Minuten expired
        ESP.restart();
    }
#endif    
  }
 
}