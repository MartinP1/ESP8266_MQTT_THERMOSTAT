ESP8366_MQTT_THERMOSTAT

# A radiator thermostat based on an ESP8266 and MQTT

## Project status
The project has been in operational use for several years.
- Temperature measurement via 3 sensors, transmission of values ​​via MQTT
- Transmission of the set temperature to the ESP via MQTT
- Control of fans that blow air from below the radiator to increase heating output (PWM)
- Schedule for changing the set temperature (implemented via an ioBroker Blockly script)
- Preferences library for device configuration via MQTT (no more hard-wired device name in the source code!)
- todo
- Window sensor undone

## Hardware overview

- ESP-based hardware - three modules are supported.
- D1 Mini (ESP8266) Issues with unstable power supply
- D1 Mini (ESP32 - since December 1, 2023)
- S2 Mini (ESP32 - since December 1, 2023) New recommendation!
