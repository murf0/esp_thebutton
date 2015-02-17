

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "debug.h"
#include "mqtt.h"
#include "gpio.h"
#include "mem.h"

#include "esphttpd.h"
#include "btn_74HC595.h"
#include "btn_mqtt.h"
#include "btn_httpd_wifi.h"
#include "btn_interrupt.h"


int btnDebounceIncrement=0;
int btnRegistered=0;
char temp[64];

void ICACHE_FLASH_ATTR btnDebounce() {
    
    if(!GPIO_INPUT_GET(0)) {
        INFO("****************\nTIMER: CHECKDEBOUNCE BTN0 Pressed\n****************\n");
        if(btnRegistered==0) {
            //Send Registermsgs
            INFO("Button: Send REGISTER to: %s\r\n", registertopic);
            os_sprintf(temp,"{\"REGISTER\": \"%s\"}\r\n", mqttcfg.client_id);
            MQTT_Publish(&mqttClient, registertopic, temp, os_strlen(temp), 2, 0);
            btnRegistered=1;
        } else {
            //Send unRegistermsgs
            INFO("Button: Send UNREGISTER to: %s\r\n", registertopic);
            os_sprintf(temp,"{\"UNREGISTER\": \"%s\"}\r\n", mqttcfg.client_id);
            MQTT_Publish(&mqttClient, registertopic, temp, os_strlen(temp), 2, 0);
            btnRegistered=0;
        }
        os_timer_disarm(&btnDebounceTimer);
        os_delay_us(1000000);
        os_timer_setfn(&btnDebounceTimer, (os_timer_func_t *)btnDebounce, NULL);
        os_timer_arm(&btnDebounceTimer, 100, 1);
    }
}

void ICACHE_FLASH_ATTR init_interupt(void) {
    INFO("Button Arm timer\n");
    os_timer_disarm(&btnDebounceTimer);
    os_timer_setfn(&btnDebounceTimer, (os_timer_func_t *)btnDebounce, NULL);
    os_timer_arm(&btnDebounceTimer, 100, 1);
}




