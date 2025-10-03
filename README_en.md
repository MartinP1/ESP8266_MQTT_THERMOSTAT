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

- 12 V DC power supply - A desktop power supply unit from [www.pollin.de](https://www.pollin.de/p/tischnetzteil-gp-1223c-12-v-2-3-a-5-5-2-1-mm-gebraucht-352200) 12V / 2.3A. 
- Danfoss actuator Thermot 230 V AC, M30x1.5 NC normally closed [088H3220](https://store.danfoss.com/de/de/Climate-Solutions-W%C3%A4rmetechnik/Warmwasser-Fu%C3%9Fbodenheizungen/Thermische-Stellantriebe/Thermischer-Stellantrieb%2C-Thermot%2C-M-30-x-1-5%2C-Versorgungsspannung-%5BV%5D-%5BAC%5D%3A-230%2C-NC-%28stromlos-geschlossen%29%2C-1-00-m/p/088H3220) approx. 10 €
  - controlled via a small relay
- 4 Arctic 80 mm housing fans ([3 Pin](https://www.arctic.de/F8/ACFAN00205A) I bought, probably [4 Pin](https://www.arctic.de/P8-PWM/ACFAN00149A) would have been the better choice) blow on the radiator from below
- 3 DS 18B20 thermal sensors from Shelly https://www.pollin.de/p/shelly-temperMQTT_PUB_DEV_PREFIXatursensor-ds18b20-591021
  - for measuring the inlet and outlet temperature of the radiator and the room temperature

![Circuit diagram](https://github.com/MartinP1/ESP8266_MQTT_THERMOSTAT/blob/main/Kicad/kicad_test_eurocard/kicad_test_eurocard.svg)

The prototype system has been decommissioned, V1 of a Kicad circuit board is currently running.
Minimum equipment would be a thermosensor for the room temperature and a switching relay for controlling the valve actuator...

## Unsorted

### What is still missing

- OTA library for the OTA update of the firmware
- Update circuit diagram change in Kicad, the DS18B20 are now supplied with 5 V to improve stability

### Improvements - what would I do differently in series production

- 4 pin fan?
- 24 V variant of the Danfoss actuator via DCDC converter with enable input?
- Full fan speed configurable (255 seems to be too fast ...)
- New PCB layout with 230 V power supply and switching relay for Danfoss actuator
- Migration of the project from Arduino IDE to PlatformIO

### Worklog

- JAN-2024 Start using the Preferences library
- MAR-2025 Various code corrections regarding the temperature sensors.
- SEP-2025 
  - ASYNC_TCP issues fixed - AsyncTCP replaced by ESP32Async 3.4.7 
  - OTA Update librariy added 
  - Window Contact


