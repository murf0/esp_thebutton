#!/bin/sh
make clean
make C99=YES
rm build/thebutton*
make 
make flash
make htmlflash
echo "Flashing"
../esptool.py -p /dev/tty.wchusbserial1410 write_flash 0x00000 firmware/0x00000.bin 0x3C000 ../blank.bin 0x40000 firmware/0x40000.bin 0x12000 webpages.espfs && screen /dev/tty.wchusbserial1410 115200

