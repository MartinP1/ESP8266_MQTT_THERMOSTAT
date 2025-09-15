#!/bin/bash

shopt -s extglob nullglob

# ..bootloader.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..partition.bin from /home/martin/.cache/arduino/sketches/<uuid>
# ..app0.bin from /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/ (3.1.1 not working)
# ..ino.bin from /home/martin/.cache/arduino/sketches/<uuid>

ls -l /home/martin/.cache/arduino/sketches

# code stolen https://stackoverflow.com/questions/28393843/bash-list-all-subdirectories-in-a-folder-write-them-to-array-to-use-in-a-menu

basedir=/home/martin/.cache/arduino/sketches

# You may omit the following subdirectories
# the syntax is that of extended globs, e.g.,
# omitdir="cmmdm|not_this_+([[:digit:]])|keep_away*"
# If you don't want to omit any subdirectories, leave empty: omitdir=
omitdir=

# Create array
if [[ -z $omitdir ]]; then
   cdarray=( "$basedir"/*/ )
else
   cdarray=( "$basedir"/!($omitdir)/ )
fi
# remove leading basedir:
cdarray=( "${cdarray[@]#"$basedir/"}" )
# remove trailing backslash and insert Exit choice
cdarray=( Exit "${cdarray[@]%/}" )

# At this point you have a nice array cdarray, indexed from 0 (for Exit)
# that contains Exit and all the subdirectories of $basedir
# (except the omitted ones)
# You should check that you have at least one directory in there:
if ((${#cdarray[@]}<=1)); then
    printf 'No subdirectories found. Exiting.\n'
    exit 0
fi

# Display the menu:
printf 'Please choose from the following. Enter 0 to exit.\n'
for i in "${!cdarray[@]}"; do
    printf '   %d %s\n' "$i" "${cdarray[i]}"
done
printf '\n'

# Now wait for user input
while true; do
    read -e -r -p 'Your choice: ' choice
    # Check that user's choice is a valid number
    if [[ $choice = +([[:digit:]]) ]]; then
        # Force the number to be interpreted in radix 10
        ((choice=10#$choice))
        # Check that choice is a valid choice
        ((choice<${#cdarray[@]})) && break
    fi
    printf 'Invalid choice, please start again.\n'
done

# At this point, you're sure the variable choice contains
# a valid choice.
if ((choice==0)); then
    printf 'Good bye.\n'
    exit 0
fi


# end code stolen 

mypath="/home/martin/.cache/arduino/sketches/"${cdarray[choice]}

echo "processing "$mypath
echo " "
stty_orig=$(stty -g)
stty -echo
read -p"Password auf Zielsystem: " var
stty $stty_orig


# ls -l $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin

# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin
# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin
# cp $mypath/ESP8266_MQTT_THERMOSTAT.ino.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/ESP8266_MQTT_THERMOSTAT.ino.bin
# cp /home/martin/.arduino15/packages/esp32/hardware/esp32/3.0.7/tools/partitions/boot_app0.bin /mnt/data/Martin/Projekte/ARDUINO/Thermostat/boot_app0.bin
# cp /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_MINI_D1_ESP32.sh /mnt/data/Martin/Projekte/ARDUINO/Thermostat/program_MINI_D1_ESP32.sh
# cp /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_esp32s2.sh /mnt/data/Martin/Projekte/ARDUINO/Thermostat/program_esp32s2.sh
echo " "
echo "tranferring from >>"$mypath"<< 
echo to >>martin@192.168.2.164:/home/martin/thermostat<<"
echo "ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin"
sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.bootloader.bin martin@192.168.2.164:/home/martin/thermostat
echo "ESP8266_MQTT_THERMOSTAT.ino.partitions.bin"
sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.partitions.bin martin@192.168.2.164:/home/martin/thermostat
echo "ESP8266_MQTT_THERMOSTAT.ino.bin"
sshpass -p $var rsync -a $mypath/ESP8266_MQTT_THERMOSTAT.ino.bin martin@192.168.2.164:/home/martin/thermostat
echo "boot_app0.bin (from board directory)"
sshpass -p $var rsync -a /home/martin/.arduino15/packages/esp32/hardware/esp32/3.3.0/tools/partitions/boot_app0.bin martin@192.168.2.164:/home/martin/thermostat
echo "program_MINI_D1_ESP32.sh (from project dir)"
sshpass -p $var rsync -a /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_MINI_D1_ESP32.sh martin@192.168.2.164:/home/martin/thermostat
echo "program_esp32s2.sh (from project dir)"
sshpass -p $var rsync -a /home/martin/github/ESP8266_MQTT_THERMOSTAT/program_esp32s2.sh martin@192.168.2.164:/home/martin/thermostat
echo "backup.sh (from project dir)"
sshpass -p $var rsync -a /home/martin/github/ESP8266_MQTT_THERMOSTAT/backup.sh martin@192.168.2.164:/home/martin/thermostat

