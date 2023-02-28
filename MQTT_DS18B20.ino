/* Martin Präkelt
MQTT Thermostat with D1 Mini Board (ESP8266)

based on below Work of Rui Santos 

28-FEB-2023 published at gitlab



*/




/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-mqtt-publish-ds18b20-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

//<<<<<<< HEAD
// includes WLAN credential define

//#define WIFI_SSID "blaba_ssid"
//#define WIFI_PASSWORD "blabla_password"

#include "secret.h"

// Raspberri Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 2, 201)
// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1888

// Temperature MQTT Topics
#define MQTT_PUB_DEV_PREFIX   "thermostat"
#define MQTT_PUB_TEMP_PREFIX MQTT_PUB_DEV_PREFIX "/sensors/"
#define MQTT_PUB_TEMP_SUFFIX "/temperature"

#define MQTT_PUB_DES_PREFIX MQTT_PUB_DEV_PREFIX "/desired"

// GPIO where the DS18B20 is connected to (16 not working, 14 and 4 tested ok...)
const int oneWireBus = 14;          
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
// Temperature value

#define MAX_NUMBER_OF_TEMP_DEVICES 3

float temp;
float desired_temp = 20.5;
DeviceAddress statDeviceAddress[8]; 

int numberOfDevices;

const char* TempsensRole[3] {"vorlauf", "ruecklauf", "raum"};

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void publishDesTemp(float partemp){
  desired_temp = partemp; 
  char buffer[10];
  sprintf(buffer,"%.2f", partemp); 
  mqttClient.publish(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX, 0, true, buffer);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  // todo not necessarily QOS 2. 1 or 0 are also ok...
  uint16_t packetIdSub = mqttClient.subscribe(MQTT_PUB_DES_PREFIX MQTT_PUB_TEMP_SUFFIX, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
  publishDesTemp(20.5);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void testDesiredTemperature(char* payload, char* topic)
{
  String strComp("thermostat/desired/temperature");
  if (strComp.compareTo(payload)!=0)
  {
    return;
  }
  desired_temp = atof(payload);

// echo message  ?
  Serial.print ("Desired temp echoed: ");  
  Serial.println ( topic);


  mqttClient.publish(topic, 1, true, payload);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println(" Publish received.");
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
  for (int i=0; i<len; i++) {
    Serial.print(payload[i]);
    
  }  
  Serial.println(" OK");

  char buffer[20];
  buffer[19]=buffer[len]='\0';
  for (int i=0;i<len && i<29;i++){
    buffer [i] = payload[i];   
  }

  /// Distinguish path
  
  testDesiredTemperature(buffer, topic);
}

void setup() {
  sensors.begin();
  Serial.begin(115200);
  Serial.println();
  
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

  Serial.print("Locating devices...");
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  
// Found device 0 with address: 28B8D281E3B53CF0




  // 
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(statDeviceAddress[i], i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(statDeviceAddress[i]);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}



void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New temperature readings
    sensors.requestTemperatures(); 
    // Temperature in Celsius degrees
    for (int i=0; i<numberOfDevices && i<3; i++)
    {
      temp = sensors.getTempCByIndex(i);
      // Temperature in Fahrenheit degrees
      //temp = sensors.getTempFByIndex(0);
    
      // Publish an MQTT message on topic esp/ds18b20/temperature
      String Topic(MQTT_PUB_TEMP_PREFIX);
      Topic += TempsensRole[i];
      Topic += String(MQTT_PUB_TEMP_SUFFIX);
      uint16_t packetIdPub1 = mqttClient.publish(Topic.c_str(), 1, true, String(temp).c_str());                            
      Serial.printf("Pubng on topic %s at QoS 1, packetId: %i ", Topic.c_str(), packetIdPub1);
      Serial.printf("Msg: %.2f \n", temp);
    }
  }
}


// function to print a device address
// 0x28 family code is printed first!
// https://cdn.sparkfun.com/datasheets/Sensors/Temp/DS18B20.pdf
// Last byte of adress is transferred first ...
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
