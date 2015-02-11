

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"
#include "debug.h"
#include "mqtt.h"
#include "wifi.h"
#include "gpio.h"
#include "mem.h"

#define ClockPin    12
#define DataPin     13
#define LatchPin    14

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
    
    GPIO_OUTPUT_SET(12,initstate); //ST_CP on SHIFT REG aka ClockPin
    GPIO_OUTPUT_SET(13,initstate); //DS pin on SHIFT REG aka DataPin
    GPIO_OUTPUT_SET(14,initstate); //SH_CP on SHIFT REG aka LatchPin
    
    
}


void ICACHE_FLASH_ATTR lightleds(int ledslit) {
    int i=0;
    GPIO_OUTPUT_SET(LatchPin, 0);
    GPIO_OUTPUT_SET(DataPin, 1);
    for(i=0; i<=8; i++) {
        GPIO_OUTPUT_SET(ClockPin, 1);
    }
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(LatchPin, 1);
}
