/*

Stub for hooking individual build defines for each Thermostat

Martin Praekelt 09-DEC-2023


*/

#define IS_FOR_PROD 0

// Temperature MQTT Topics
#if IS_FOR_PROD 
#define MQTT_PUB_DEV_PREFIX   "thermostat"
#else
#define MQTT_PUB_DEV_PREFIX   "test_thermostat"
#endif

#include "..//MQTT_DS18B20.ino"