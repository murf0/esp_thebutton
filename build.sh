#!/bin/sh
make clean
make C99=YES
rm build/thebutton*
make
make flash
make htmlflash
