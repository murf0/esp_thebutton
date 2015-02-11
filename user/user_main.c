

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"

#include "mqtt.h"
#include "esphttpd.h"
#include "74HC595.h"

#include "httpd_wifi.h"

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
    ioInit();
    initShiftIO();
    lightleds(0);
    httpdInit(builtInUrls, 80);
    init_mqtt();
    os_printf("\nReady\n");
}
