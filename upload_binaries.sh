#!/bin/bash
# ..bootloader.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..partition.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..app0.bin from /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/ (3.1.1 not working)
# ..ino.bin from /home/martin/.cache/arduino/sketches/<uuid>

cd /home/martin/.cache/arduino/sketches
ls -l

read -p "Welches Image: " var
mypath="/home/martin/.cache/arduino/sketches/"$var
echo "processing "$mypath

read -p "Password auf Zielsystem: " var


# ls -l $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin

# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin
# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin
# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.bin
# cp /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/boot_app0.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/boot_app0.bin
# cp /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_MINI_D1_ESP32.sh /mnt/data/Martin/Projekte/ARDUINO/Thermostat/program_MINI_D1_ESP32.sh
# cp /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_esp32s2.sh /mnt/data/Martin/Projekte/ARDUINO/Thermostat/program_esp32s2.sh

sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin martin@192.168.2.164:/home/martin/thermostat
sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin martin@192.168.2.164:/home/martin/thermostat
sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.bin martin@192.168.2.164:/home/martin/thermostat
sshpass -p $var rsync -a /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/boot_app0.bin martin@192.168.2.164:/home/martin/thermostat
sshpass -p $var rsync -a /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_MINI_D1_ESP32.sh martin@192.168.2.164:/home/martin/thermostat
sshpass -p $var rsync -a /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_esp32s2.sh martin@192.168.2.164:/home/martin/thermostat

