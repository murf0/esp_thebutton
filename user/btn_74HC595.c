

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

#define delay_ms 1001
int countdown=0,flashledslit=0;
static os_timer_t delay_timer;

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
/*
void ICACHE_FLASH_ATTR flashleds_NO(int times) {
    ets_wdt_disable();
    ets_wdt_feed();
    int y=0,i=0;
    for(i=1;i<=times;i++) {
        for(y=0;y<=8;y++) {
            os_delay_us(100000);
            lightleds(y);
        }
        //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
        os_delay_us(100000);
        lightleds(0);
    }
    ets_wdt_enable();
    //INFO("Flashleds %d done\n",times);
}
 start timer + set countdown
 in timer func countdown integer flash round sleep 1000000
 if countdown != 0 arm timer
 
 */
void ICACHE_FLASH_ATTR flashled() {
    os_timer_disarm(&delay_timer);
    if(flashledslit >= 8) {
        flashledslit=0;
    }
    lightleds(flashledslit);
    flashledslit+=1;
    countdown-=1;
    if(!(countdown <=0)) {
        os_timer_arm(&delay_timer, delay_ms, 1);
    }
}

void ICACHE_FLASH_ATTR flashleds(int times) {
    countdown = times*8;
    os_timer_setfn(&delay_timer, (os_timer_func_t*) &flashled,0);
    os_timer_arm(&delay_timer, delay_ms, 0);
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
