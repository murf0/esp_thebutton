

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "debug.h"
#include "mqtt.h"
#include "wifi.h"
#include "gpio.h"
#include "mem.h"

#include "esphttpd.h"
#include "btn_74HC595.h"
#include "btn_mqtt.h"
#include "btn_httpd_wifi.h"

static ETSTimer btnDebounceTimer;

int btnDebounceIncrement=0;
int btnRegistered=0;
char temp[64];

void btnDebounce() {
    os_timer_disarm(&btnDebounceTimer);
    INFO("****************\nTIMER: CHECKDEBOUNCE\n****************\n");
    if(btnDebounceIncrement>0) {
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
        os_delay_us(500000);
        btnDebounceIncrement=0;
    }
    os_timer_setfn(&btnDebounceTimer, (os_timer_func_t *)btnDebounce, NULL);
    os_timer_arm(&btnDebounceTimer, 10, 0);
}
void btn_interupt(uint8_t key) {
    btnDebounceIncrement+=1;
}

void ICACHE_FLASH_ATTR init_interupt(void) {
    INFO("Button Arm timer\n");
    os_timer_disarm(&btnDebounceTimer);
    os_timer_setfn(&btnDebounceTimer, (os_timer_func_t *)btnDebounce, NULL);
    os_timer_arm(&btnDebounceTimer, 3000, 0);
    
    ETS_GPIO_INTR_DISABLE(); // Disable gpio interrupts
    
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    //PIN_PULLDWN_DIS(PERIPHS_IO_MUX_MTCK_U); // disable pullodwn
    //PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U); // pull - up pin
    
    
    ETS_GPIO_INTR_ATTACH(btn_interupt, 0); // GPIO0 interrupt handler
    gpio_pin_intr_state_set(GPIO_ID_PIN(0), GPIO_PIN_INTR_LOLEVEL); //GPIO_PIN_INTR_LOLEVEL GPIO_PIN_INTR_POSEDGE
    ETS_GPIO_INTR_ENABLE(); // Enable gpio interrupts

}




