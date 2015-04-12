#!/bin/sh
make clean
make HTTPD=TRUE
make MQTT=TRUE
make BUTTON=TRUE

echo "Flashing"
make flash && screen /dev/tty.wchusbserial14* 115200

#/Volumes/esp-open-sdk/esptool/esptool.py -p /dev/tty.wchusbserial14* write_flash 0x00000 firmware/0x00000.bin 0x3c000 firmware/0x3c000.bin 0x12000 webpages.espfs && screen /dev/tty.wchusbserial14* 115200
