

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"
#include "debug.h"
#include "mqtt.h"

#include "gpio.h"
#include "mem.h"


#define DataPin     14 //DS, DataPin
#define LatchPin    12 //ST_CP, LatchPin
#define ClockPin    13 //SH_CP, ClockPin


void ICACHE_FLASH_ATTR initShiftIO(void) {
    int initstate = 0;
    
    gpio_init();
    //Set GPIOS to output mode
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_XPD_DCDC_U, FUNC_GPIO16);
    
    GPIO_OUTPUT_SET(12,0); //SH_CP, ClockPin
    GPIO_OUTPUT_SET(13,0); //DS, DataPin
    GPIO_OUTPUT_SET(14,0); //ST_CP, LatchPin
    
    
}
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
    GPIO_OUTPUT_SET(LatchPin, 0);
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(ClockPin, 0);
}

void ICACHE_FLASH_ATTR lightleds(int ledslit) {
    int i;
    noleds();
    INFO("Set %d LEDS\n",ledslit);
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

}
void ICACHE_FLASH_ATTR flashleds() {
    int i,y,ledslit;
    for(i=0;i<40;i++) {
        ledslit=0;
        INFO("I: %d\n",i);
        for(y=0;y<=8;y++) {
            INFO("Y: %d\n",y);
            os_delay_us(50000);
            lightleds(ledslit);
            ledslit++;
            os_delay_us(50000);
        }
    }
    lightleds(0);
}
void ICACHE_FLASH_ATTR BTN_Task(os_event_t *e) {
    switch(e->par){
        case 1:
            flashleds();
            break;
        case 2:
            noleds();
            break;
        case 3:
            lightleds(4);
            break;
    }
}
