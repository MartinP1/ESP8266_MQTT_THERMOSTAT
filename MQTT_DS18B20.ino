
#include "GLOBAL_VARS_MQTT_DS18B20.h"
#include "unspecialized_mqtt_doings.h"
#include "desired_temp_MQTT_DS18B20.h"
#include "desired_speed_MQTT_DS18B20.h"
#include "received_temp_MQTT_DS18B20.h"
#include "fan_pwm_MQTT_DS18B20.h"
// #include "ventile_control_MQTT_DS18B20.h"
// #include "window_contact_MQTT_DS18B20.h"
#include "do_calculations_MQTT_DS18B20.h"

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  // todo not necessarily QOS 2. 1 or 0 are also ok...
  uint16_t packetIdSub = mqttClient.subscribe(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX, 2);
  Serial.print("Subscribing desired temp at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  publishDesTemp(desired_temp);
  packetIdSub = mqttClient.subscribe(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX, 2);
  Serial.print("Subscribing desired hyst at QoS 2, packetId: ");
  publishTempHyst(temp_hyst);
  packetIdSub = mqttClient.subscribe(MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX, 2);
  Serial.print("Subscribing max fan speed at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  publishDesSpeed(throttleFanspeed);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  // Serial.println("Subscribe acknowledged.");
  // Serial.print("  packetId: ");
  // Serial.println(packetId);
  // Serial.print("  qos: ");
  // Serial.println(qos);
}
// 
void onMqttUnsubscribe(uint16_t packetId) {
  // Serial.println("Unsubscribe acknowledged.");
  // Serial.print("  packetId: ");
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
}

void setup() {
  sensors.begin();
  Serial.begin(115200);
  Serial.println();
  pinMode(pwmGpio, OUTPUT);
  analogWriteFreq(8000);
  analogWrite(pwmGpio,255); // raw write off!
  pinMode(valveGpio, OUTPUT);
  pinMode(windowContact, INPUT);  
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
   mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  connectToWifi();
  initTemperatureSensors();
}



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

    
  }
}


