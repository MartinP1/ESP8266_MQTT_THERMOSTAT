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
// #include <AsyncMqttClient.h>
#include <AsyncMqtt_Generic.h>
#include <LibPrintf.h>
// set to 1 if deployed to real target

#define IS_FOR_PROD 1
#define SERIAL_TRACE 0
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
#define MQTT_PUB_TEMPHYST_SUFFIX "/temp_hyst"

#define MQTT_PUB_ACTOR_PREFIX MQTT_PUB_DEV_PREFIX "/actors"
#define MQTT_PUB_DES_PREFIX MQTT_PUB_DEV_PREFIX "/desired"

#define MQTT_PUB_FANTHROTTLE_SUFFIX "/throttle_fanspeed"
#define MQTT_PUB_FANACT_SUFFIX "/actual_fanspeed"

#define MQTT_PUB_VALVE_SUFFIX "/valve"
// GPIO where the DS18B20 is connected to (16 not working, 14 and 4 tested ok...)
const int oneWireBus = 14; 
OneWire oneWire(oneWireBus);

// pwm output
const int pwmGpio = 2;          
// valve 
const int valveGpio = 5;
// window contact
const int windowContact = 4;

DallasTemperature sensors(&oneWire);
// Temperature value

#define MAX_NUMBER_OF_TEMP_DEVICES 3
float desired_temp = 21.7;
float temp_hyst = 0.15; // +/- 0.15 °C difference to desired_temp allowed

#define PWM_OFF 0

#define PWM_FULL 255

uint8_t PWM_THROTTLE=70;

uint8_t pwmActual;
bool ventState;

// global variable for MQTT comms

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings and do thermostate calculations


