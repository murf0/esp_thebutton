

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"

#include "../lib/libesphttpd/include/httpd.h"
#include "../lib/libesphttpd/include/cgiwifi.h"
#include "../lib/libesphttpd/include/httpdespfs.h"

#include "driver/uart.h"
#include "btn_74HC595.h"
#include "btn_mqtt.h"
#include "btn_wifi_handler.h"
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


//Main routine. Initialize stdout, the I/O, webserver,Mqtt and we're done.
void user_init(void) {
    //stdoutInit();
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    wifi_set_event_handler_cb(btn_wifi_handle_event_cb);
    int x=wifi_get_opmode();
    if (x==3 || x==2) {
        btnSetSoftAP();
        INFO("\r\n\r\nINITHTTPD\n");
        espFsInit((void*)(0x40200000 + 0x12000));
        httpdInit(builtInUrls, 80);
    } else {
        INFO("\r\n\r\nNot SOFTAP, Not initating Httpd\n");
    }
    noleds();
    INFO("\ninitmqtt\n");
    init_mqtt();
    INFO("\nbtnInitIO\n");
    btnInitIO();
    os_printf("\nReady\n");
    //system_print_meminfo();
    //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
}
