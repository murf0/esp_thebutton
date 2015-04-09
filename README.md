***esp_thebutton***
==========
https://thebutton.se

**Features:**
* Sending messages when button is pressed
* Receiving messages adding and removing LEDS 

**Building:**
Clone project: 
```bash
git clone https://github.com/murf0/esp_mqtt.git
```
Checkout Submodules:
```bash
git submodule init
git submodule update
```

Patch mkespfsimage:
```bash
cd lib && patch -p0 < mkespfsimage.patch && cd ..
```

Build & Flash:
```bash
./build.sh
```

Update Submodules:
```bash
git submodule foreach git pull origin master
```


Clear out Settings (AP and such):
Write blank.bin to address 0x7e000
```bash
esptool.py -p /dev/tty.wchusbserial1410 write_flash 0x7e000 ../blank.bin
```

Issues:
* Random Reboots.
* java_thebutton isn't finished.
