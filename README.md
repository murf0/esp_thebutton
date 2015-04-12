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
```bash
make resetflash
```
or
```
make hardresetflash
```
to clear out AP-settings also


FAQ:

if you're getting the error at initmqtt: 
```
client handshake start.
No heap available, failed to malloc 2064
Fatal exception (29):
epc1=0x4000e1b2, epc2=0x00000000, epc3=0x00000000, excvaddr=0x00000004, depc=0x00000000
```
Then the ssl-certificate is more than 1024 bits..
(i've successfully used 2048bits, 4096 fails)
