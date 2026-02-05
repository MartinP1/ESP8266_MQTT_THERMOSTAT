# Summary of used libraries etc for building releases

Scan build output copied from arduino output window to file with

```martin@martin-D2836-S1:~/Temp$ cat Arduino-build.log | grep -i -e "using board" -e "using library" >used_libs.txt
```
Paste content of used_libs.txt to wiki article

## 1.2.4 06-OCT-2025

```
Using board 'lolin_s2_mini' from platform in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0
Using library Preferences at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/Preferences 
Using library DallasTemperature at version 4.0.5 in folder: /home/martin/Arduino/libraries/DallasTemperature 
Using library OneWire at version 2.3.8 in folder: /home/martin/Arduino/libraries/OneWire 
Using library WiFi at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/WiFi 
Using library Networking at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/Network 
Using library ESPmDNS at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/ESPmDNS 
Using library ArduinoOTA at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/ArduinoOTA 
Using library Update at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/Update 
Using library espMqttClient at version 1.7.1 in folder: /home/martin/Arduino/libraries/espMqttClient-1.7.1 
Using library NetworkClientSecure at version 3.3.0 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/libraries/NetworkClientSecure 
Using library Async TCP at version 3.4.9 in folder: /home/martin/Arduino/libraries/Async_TCP 
```

## 1.2.6 5-FEB-2026

Using board 'lolin_s2_mini' from platform in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6
Using library Preferences at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/Preferences 
Using library DallasTemperature at version 4.0.5 in folder: /home/martin/Arduino/libraries/DallasTemperature 
Using library OneWire at version 2.3.8 in folder: /home/martin/Arduino/libraries/OneWire 
Using library WiFi at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/WiFi 
Using library Networking at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/Network 
Using library ESPmDNS at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/ESPmDNS 
Using library ArduinoOTA at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/ArduinoOTA 
Using library Update at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/Update 
Using library espMqttClient at version 1.7.1 in folder: /home/martin/Arduino/libraries/espMqttClient-1.7.1 
Using library NetworkClientSecure at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/NetworkClientSecure 
Using library Hash at version 3.3.6 in folder: /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.6/libraries/Hash 
Using library Async TCP at version 3.4.10 in folder: /home/martin/Arduino/libraries/Async_TCP 