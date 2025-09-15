#!/bin/bash
# installation on debiantest LXC
#=============
# activate venv
source /home/martin/.local/pipx/venvs/esptool/bin/activate
#=============
# do programming
python3 "/home/martin/.local/pipx/venvs/esptool/bin/esptool.py" \
  --chip esp32s2 --port "/dev/ttyACM0" --baud 921600  --before default_reset \
 --after hard_reset write_flash  -z --flash_mode keep --flash_freq keep --flash_size keep \
 0x1000 "./ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin" \
 0x8000 "./ESP8266_MQTT_THERMOSTAT.ino.partitions.bin" \
 0xe000 "./boot_app0.bin" \
 0x10000 "./ESP8266_MQTT_THERMOSTAT.ino.bin"

# ..bootloader.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..partition.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..app0.bin from /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/ (3.1.1 not working)
# ..ino.bin from /home/martin/.cache/arduino/sketches/<uuid>

# new use boot_app0.bin from /home/martin/.arduino15/packages/esp32/hardware/esp32/3.2.0/tools/partitions/