/* 
 * Adaption By Murf
*/
#include "btn_mqtt.h"

#include "ets_sys.h"
#include "osapi.h"
#include "debug.h"
#include "mqtt.h"
#include "gpio.h"
#include "mem.h"
#include "btn_interrupt.h"

static ETSTimer btnWiFiLinker;

MQTTCFG mqttcfg;
int ledslit=0;

void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status) {
    os_timer_disarm(&btnWiFiLinker);
    if(wifi_station_get_connect_status() == STATION_GOT_IP){
        MQTT_Connect(&mqttClient);
        INFO("****************\nTIMER: Connect MQTT\n****************\n");
        //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
    } else {
        MQTT_Disconnect(&mqttClient);
        INFO("TIMER: Disconnect MQTT\n");
        //INFO("FREE HEAP: %d\n",system_get_free_heap_size());
        os_timer_setfn(&btnWiFiLinker, (os_timer_func_t *)wifiConnectCb, NULL);
        os_timer_arm(&btnWiFiLinker, 2000, 0);
    }
}

void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
    os_sprintf(registertopic,"thebutton/cb/%s/register",mqttcfg.client_id);
    os_sprintf(statustopic,"thebutton/cb/%s/set",mqttcfg.client_id);
    
	MQTT_Subscribe(client, statustopic, 2);
    
    INFO("MQTT: Connected! subscribe to: %s\r\n", statustopic);
    flashleds(2);
    system_os_task(BTN_Task, BTN_TASK_PRIO, btn_procTaskQueue, BTN_TASK_QUEUE_SIZE);
}

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
    lightleds(8);
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}
void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len) {
    char *topicBuf = (char*)os_zalloc(topic_len+1),
    *dataBuf = (char*)os_zalloc(data_len+1);
    MQTT_Client* client = (MQTT_Client*)args;
    os_memcpy(topicBuf, topic, topic_len);
    topicBuf[topic_len] = 0;
    os_memcpy(dataBuf, data, data_len);
    dataBuf[data_len] = 0;
    INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
    /*PARSE MESSG
     Commands available is:
     {\"DO\":\"ADDLED\"}
     {\"DO\":\"REMOVELED\"}
     {\"DO\":\"NOLED\"}
     {\"DO\":\"FLASHLED\"}
    */
    //Here we do logic...
    if(os_strcmp(dataBuf,"{\"DO\":\"ADDLED\"}")==0) {
        if(ledslit!=8) {
            ledslit+=1;
        }
        lightleds(ledslit);
        INFO("ADDLED - Ledslit: %d\n",ledslit);
    } else if(os_strcmp(dataBuf,"{\"DO\":\"REMOVELED\"}")==0) {
        if(ledslit!=0) {
            ledslit-=1;
        }
        lightleds(ledslit);
        INFO("REMOVELED - Ledslit: %d\n",ledslit);
    } else if(os_strcmp(dataBuf,"{\"DO\":\"NOLED\"}")==0) {
        ledslit=0;
        lightleds(ledslit);
        INFO("NOLED - Ledslit: %d\n",ledslit);
    } else if(os_strcmp(dataBuf,"{\"DO\":\"FLASHLED\"}")==0) {
        flashleds(10);
        INFO("FLASHLED - Ledslit: %d\n",ledslit);
    }
    
    os_free(topicBuf);
    os_free(dataBuf);
}

void ICACHE_FLASH_ATTR init_mqtt(void) {
    os_sprintf(mqttcfg.mqtt_host, "%s", "mqtt.murf.se");
    os_sprintf(mqttcfg.mqtt_user, "%s", "thebutton");
    os_sprintf(mqttcfg.mqtt_pass, "%s", "thebutton");
    INFO("CHIPID: %08X\n",system_get_chip_id());
    os_sprintf(mqttcfg.client_id, "%08X", system_get_chip_id());
    mqttcfg.mqtt_port=8885;
    mqttcfg.mqtt_keepalive=120;
    mqttcfg.security = 1; // 1=ssl (max 1024bit certificate) 0=nonssl
    INFO("host: %s Port: %d Security: %d \n", mqttcfg.mqtt_host, mqttcfg.mqtt_port, mqttcfg.security);
	MQTT_InitConnection(&mqttClient, mqttcfg.mqtt_host, mqttcfg.mqtt_port, mqttcfg.security);
    
    INFO("ClientId: %s UserID: %s Password: %s Keepalive: %d\n ", mqttcfg.client_id, mqttcfg.mqtt_user, mqttcfg.mqtt_pass, mqttcfg.mqtt_keepalive);
	MQTT_InitClient(&mqttClient, mqttcfg.client_id, mqttcfg.mqtt_user, mqttcfg.mqtt_pass, mqttcfg.mqtt_keepalive, 1);
    
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
	
    //MQTT_Connect(&mqttClient);
    //Timer to make sure the MQTTclient is connected
    INFO("MQtt Arm timers?\n");
    os_timer_disarm(&btnWiFiLinker);
    os_timer_setfn(&btnWiFiLinker, (os_timer_func_t *)wifiConnectCb, NULL);
    os_timer_arm(&btnWiFiLinker, 2000, 0);
}
