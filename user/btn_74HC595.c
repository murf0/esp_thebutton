

/*
* Murf
*/

#include "ets_sys.h"
#include "osapi.h"
#include "debug.h"
#include "mqtt.h"

#include "gpio.h"
#include "mem.h"
#include "btn_74HC595.h"

void ICACHE_FLASH_ATTR noleds() {
    //INFO("Set NoLEDS\n");
    int i=0;
    GPIO_OUTPUT_SET(DataPin, 0);
    for(i=0; i<=8; i++) {
        GPIO_OUTPUT_SET(ClockPin, 1);
        GPIO_OUTPUT_SET(ClockPin, 0);
    }
    GPIO_OUTPUT_SET(LatchPin, 1);
    //Reset pins to OFF
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(ClockPin, 0);
    GPIO_OUTPUT_SET(LatchPin, 0);
}
void ICACHE_FLASH_ATTR flashleds(int times) {
    ets_wdt_disable();
    int y=0,i=0;
    for(i=1;i<=times;i++) {
        for(y=0;y<=8;y++) {
            wdt_feed();
            os_delay_us(Flashtime);
            lightleds(y);
        }
        //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
        wdt_feed();
        os_delay_us(Flashtime);
        lightleds(0);
    }
    wdt_feed();
    os_delay_us(Flashtime);
    ets_wdt_enable();
    INFO("Flashleds %d done\n",times);
}


void ICACHE_FLASH_ATTR lightleds(int ledslit) {
    int i=0;
    noleds();
    //INFO("Set %d LEDS\n",ledslit);
    GPIO_OUTPUT_SET(DataPin, 1);
    for(i=1; i<=ledslit; i++) {
        GPIO_OUTPUT_SET(ClockPin, 1);
        GPIO_OUTPUT_SET(ClockPin, 0);
    }
    GPIO_OUTPUT_SET(LatchPin, 1);
    //Reset pins to OFF
    GPIO_OUTPUT_SET(LatchPin, 0);
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(ClockPin, 0);

    //INFO("LIGHTLEDS %d done\n",ledslit);
}
