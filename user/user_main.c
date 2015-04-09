

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

/*
#define BTN_TASK_PRIO        		1
#define BTN_TASK_QUEUE_SIZE         1

os_event_t btn_procTaskQueue[BTN_TASK_QUEUE_SIZE];
*/
HttpdBuiltInUrl builtInUrls[]={
    {"/", cgiRedirect, "/wifi.tpl"},
    {"/wifiscan.cgi", cgiWiFiScan, NULL},
    {"/wifi.tpl", cgiEspFsTemplate, btnTplWlan},
    {"/connect.cgi", cgiWiFiConnect, NULL},
    {"/setmode.cgi", cgiWifiSetMode, NULL},
    
    {"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
    {NULL, NULL, NULL}
};


//Main routine. Initialize stdout, the I/O and the webserver and we're done.
void user_init(void) {
    stdoutInit();
    os_delay_us(1000);
    btnSetSoftAP();
    INFO("\nINITHTTPD\n");
    httpdInit(builtInUrls, 80);
    INFO("\ninitmqtt\n");
    init_mqtt();
    INFO("\nbtnInitIO\n");
    btnInitIO();
    //Set all pins on shiftreg to 0
    noleds();
    os_printf("\nReady\n");
    system_print_meminfo();
    INFO("FREE HEAP: %d\n",system_get_free_heap_size());
}
