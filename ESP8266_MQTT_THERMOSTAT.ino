#define VERSION_NO "1.2.4 06-OCT-2025"
#include "./GLOBAL_VARS_MQTT_DS18B20.h"
#include "unspecialized_mqtt_doings.h"
#include "MqttLogging.h"
#include "desired_temp_MQTT_DS18B20.h"
#include "desired_speed_MQTT_DS18B20.h"
#include "received_temp_MQTT_DS18B20.h"
#include "fan_pwm_MQTT_DS18B20.h"
#include "window_contact_MQTT_DS18B20.h"
#include "do_calculations_MQTT_DS18B20.h"
#if USE_MQTT_BINDING_CLASS
#include "MqttBindings.h"
#endif
#if !defined(ARDUINO_D1_MINI32) && !defined(ARDUINO_LOLIN_S2_MINI) && !defined(ARDUINO_ESP8266_WEMOS_D1MINI)
#error "unsupported board type"
#error ARDUINO_BOARD
#else
#pragma message ARDUINO_BOARD is selected
#endif

#include "thermostat_preferences.h"

void onMqttConnect(bool sessionPresent) {
  //DumpFreeRAM();
  Serial.print("Connected to MQTT - IP=");
  Serial.print(WiFi.localIP());
  Serial.print("- RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.print(" dB - Session present: ");
  Serial.print(sessionPresent);
  Serial.println("StackFree: ");
  delay(200);
  // todo not necessarily QOS 2, 1 or 0 are also ok... 
#if 0
  // try to subscribe a complete tree with all leafs does nont work with bertmelis/espMqttClient
  uint16_t packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX).c_str(), 2);
  Serial.print("Subscribing desired tree at QOS2, packetId: ");
  Serial.println(packetIdSub);
#else
  uint16_t packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX).c_str(), 2);
  Serial.print("Subscribing desired temp at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  #if MQTT_PUBLISH_SUBSCRIBED
  publishDesTemp(desired_temp); // must be created by mqtt broker
  #endif
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_TEMPHYST_SUFFIX).c_str(), 2);
  Serial.print("Subscribing desired hyst at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  #if MQTT_PUBLISH_SUBSCRIBED
  publishTempHyst(temp_hyst); // must be created by mqtt broker
  #endif
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DES_PREFIX MQTT_PUB_FANTHROTTLE_SUFFIX).c_str(), 2);
  Serial.print("Subscribing max fan speed at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  #if MQTT_PUBLISH_SUBSCRIBED
  publishDesSpeed(PWM_THROTTLE); //must be created by broker (?)
  #endif
#endif  
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DEV_PREFIX +"/Preferences/MqttName").c_str(), 2);
  Serial.print("Subscribing desired device name, packetId: ");
  Serial.println(packetIdSub);
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DEV_PREFIX +"/Preferences/Debug").c_str(), 2);
  Serial.print("Subscribing debug setting, packetId: ");
  Serial.println(packetIdSub);
  packetIdSub = mqttClient.subscribe((MQTT_PUB_DEV_PREFIX +"/Preferences/OverrideWindowsSensor").c_str(), 2);
  Serial.print("Subscribing OverrideWindowsSensor, packetId: ");
  Serial.println(packetIdSub);
  
  for (int i=0;i<numberOfDevices; i++){
    publishSensorState(i);
    is_old_valid[i]=is_valid[i]; // reporting of state fixed to one time
  }
#if 1
  char* pBuff=log_buffer;
  int written = snprintf (pBuff,119,"Thermostat %s started, %d sensors", VERSION_NO, numberOfDevices);
  int DevIdx=0;
  while (DevIdx<numberOfDevices){ 
    if (written>0){
      pBuff+=written;    
      written = snprintf (pBuff,19,", %02X%02X%02X%02X%02X%02X/%02X", 
        // statDeviceAddress[DevIdx][7], <- skipped, CRC
        statDeviceAddress[DevIdx][6], // <- MSByte of serial number
        statDeviceAddress[DevIdx][5],
        statDeviceAddress[DevIdx][4],
        statDeviceAddress[DevIdx][3],
        statDeviceAddress[DevIdx][2], 
        statDeviceAddress[DevIdx][1], // <- LSByte of serial number
        statDeviceAddress[DevIdx][0]); // <- family code set to 0x28
    } 
    else
      break;
    DevIdx++;
  }  
  MQTTLog(log_buffer);
     
#else
  MQTTLogPrintf("Thermostat started %d Thermosensors found ", numberOfDevices);
#endif  
}

#if USE_BERTMELIS
void onMqttDisconnect(espMqttClientTypes::DisconnectReason reason) {
  Serial.printf("Disconnected from MQTT: %u.\n", static_cast<uint8_t>(reason));

/*  if (WiFi.isConnected()) {
    reconnectMqtt = true;
    lastReconnect = millis();
  }*/
}

#else
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  // if (WiFi.isConnected()) {
  //  mqttReconnectTimer.once(2, connectToMqtt);
  // }
  return;
  }
#endif

#if USE_BERTMELIS
void onMqttSubscribe(uint16_t packetId, const espMqttClientTypes::SubscribeReturncode* codes, size_t len) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  for (size_t i = 0; i < len; ++i) {
    Serial.print("  qos: ");
    Serial.println(static_cast<uint8_t>(codes[i]));
  }
}
#else
void onMqttSubscribe(const uint16_t& packetId, const uint8_t& qos){
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
#endif

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

#if USE_BERTMELIS 
void onMqttMessage(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total) {
#else
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
#endif
  char buffer[50];
  if (len>=50)
    len=49;
  for (int i=0;i<len;i++){
    buffer [i] = payload[i];   
  buffer[len]='\0';
  }
  Serial.print("Message received, len=");
  Serial.print(buffer);
  /// Distinguish pathpayload
  testPreferences(buffer, topic);
  testDesiredTemperature(buffer, topic);  
  testDesiredTempHyst(buffer, topic);
  testDesiredFanspeed(buffer,topic);
#if USE_MQTT_BINDING_CLASS
  tempHystNode.TestReceived(topic, buffer);
#endif

}






#if 0 
/* some watchdog settings */
#include "soc/rtc_wdt.h"
void doWatchdogSettings(){
  rtc_wdt_protect_off();
  rtc_wdt_disable();
}


#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

void doWatchdogSettings(){
  TIMERG1.wdtwprotect = TIMG_WDT_WKEY_VALUE; // Unlock timer config.
  TIMERG1.wdtfeed = 1; // Reset feed count.
  TIMERG1.wdtconfig0.en = 0; // Disable timer.
  TIMERG1.wdtwprotect = 0; // Lock timer config.

  TIMERG0.wdtwprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdtfeed = 1;
  TIMERG0.wdtconfig0.en = 0;
  TIMERG0.wdtwprotect = 0;
}  
/* END: some hardware watchdog settings */
#endif
//SET_LOOP_TASK_STACK_SIZE(16 * 1024);  // 16KB (default 8 kB)

void setup() {
  // not working stays on 9600 baud ...Serial.begin(115200);
  Serial.begin();
  delay(4000);
  yield();
  Serial.println("*** setup ***");
  sensors.begin();
  // doWatchdog();
  // delay(4000); // give some time to startup comms 
  // Serial.println("after sensors.begin()");
  yield();
  getPreferences();
  //setMQTTBindings();
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  // ledcSetup(0, 8000, 8); // pwm#, freq, resolution(bits)
  // ledcAttachPin(pwmGpio, 0);
  ledcAttachChannel(pwmGpio, 8000, 8, 0);
#endif
#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  pinMode(pwmGpio, OUTPUT);
  analogWriteFreq(8000);
  analogWrite(pwmGpio,255); // raw write off!
#endif  
  pinMode(valveGpio, OUTPUT);
  pinMode(windowContact, INPUT_PULLUP); 
  pinMode(oneWireBus, INPUT_PULLUP);
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  WiFi.onEvent(onWifiConnect,WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(onWifiDisconnect,WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  yield();
  // doWatchdog();
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
  yield();
  
  // mqttClient.setKeepAlive(30);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USE92:de:dc:42:f3:f8R", "REPLACE_WITH_YOUR_PASSWORD");
  // doWatchdog();
  Serial.println("issue connectToWifi()");
  connectToWifi();
  initTemperatureSensors();
  previousMillis = millis(); // avoid initial overrun in loop
#if OTA_UPDATES
  // Same as MQTT device name
  ArduinoOTA.setHostname(MQTT_PUB_DEV_PREFIX.c_str());
  // from secret.h
  ArduinoOTA.setPassword(OTA_PASSWORD); 
  yield();
  ArduinoOTA.begin(); 
  yield();
#endif
  Serial.println("setup() finished");
}


uint8_t lastPwm=0; 

void SelftestPwm(){
   if (lastPwm != PWM_THROTTLE) {
    lastPwm = PWM_THROTTLE;
    setSpeed(PWM_THROTTLE);
    Serial.print("set PWM to ");
    Serial.println(PWM_THROTTLE);
   }

}


unsigned short usWifiDown=12;
void loop() {
  unsigned long currentMillis = millis();
  yield();
  if (uiDebug == 1) {
    SelftestPwm();
    return;
  }
#if OTA_UPDATES
  yield();
  ArduinoOTA.handle();
  yield();
#endif
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New temperature readings
    if (!wifiStatus()) return; // checks MQTT and wifi up, if not, do not try do do anything ...
    // wifiStatus();
    GetWindowState(); // stored in global variable 
    yield();


    getTemperatures();
    yield();

    runTempControl();
#if 1
// try it again ... does not help ...
    if (WiFi.isConnected() && mqttClient.connected())
    {
      usWifiDown=12;  // 2 minutes @ 10 sec run interval
    }
    else
    {
      usWifiDown--;
      Serial.print(" Wifi Error count down ");
      Serial.println(usWifiDown);
      if (usWifiDown==0) // 2 Minutes expired
        ESP.restart();
    }
#endif    
  }
 
}