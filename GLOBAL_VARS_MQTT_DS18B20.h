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

// set to 1 if deployed to real target

#define IS_FOR_PROD 0
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
#if IS_FOR_PROD 
#define MQTT_PUB_DEV_PREFIX   "thermostat"
#else
#define MQTT_PUB_DEV_PREFIX   "test_thermostat"
#endif
#define MQTT_PUB_TEMP_PREFIX MQTT_PUB_DEV_PREFIX "/sensors/"
#define MQTT_PUB_TEMP_SUFFIX "/temperature"

#define MQTT_PUB_DES_PREFIX MQTT_PUB_DEV_PREFIX "/desired"

#define MQTT_PUB_FANMAX_SUFFIX "/max_fansspeed"
// GPIO where the DS18B20 is connected to (16 not working, 14 and 4 tested ok...)
const int oneWireBus = 14;
const int pwmGpio = 4;          
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
// Temperature value

#define MAX_NUMBER_OF_TEMP_DEVICES 3


// global variable for MQTT comms

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


