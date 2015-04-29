

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"


#include "esphttpd.h"
#include "btn_74HC595.h"
#include "btn_mqtt.h"
#include "btn_httpd_wifi.h"

#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
//HTTPD
#include "stdout.h"
#include "httpd.h"

HttpdBuiltInUrl builtInUrls[]={
    {"/", cgiRedirect, "/wifi.tpl"},
    {"/wifiscan.cgi", cgiWiFiScan, NULL},
    {"/wifi.tpl", cgiEspFsTemplate, btnTplWlan},
    {"/connect.cgi", cgiWiFiConnect, NULL},
    {"/setmode.cgi", cgiWiFiSetMode, NULL},
    
    {"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
    {NULL, NULL, NULL}
};


//Main routine. Initialize stdout, the I/O and the webserver and we're done.
void user_init(void) {
    stdoutInit();
    os_delay_us(500000);
    int x=wifi_get_opmode();
    if (x==3 || x==2) {
        btnSetSoftAP();
        INFO("\nINITHTTPD\n");
        espFsInit((void*)(0x40200000 + 0x12000));
        httpdInit(builtInUrls, 80);
    } else {
        INFO("Not SOFTAP, Not initating Httpd");
    }
    INFO("\ninitmqtt\n");
    init_mqtt();
    INFO("\nbtnInitIO\n");
    btnInitIO();
    //Set all pins on shiftreg to 0
    noleds();
    lightleds(8);
    os_printf("\nReady\n");
    //system_print_meminfo();
    //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
}
