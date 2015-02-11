/* 
 * Adaption By Murf
*/
#include "ets_sys.h"
#include "osapi.h"
#include "config.h"
#include "debug.h"
#include "mqtt.h"
#include "wifi.h"
#include "gpio.h"
#include "mem.h;

static ETSTimer btnWiFiLinker;

MQTT_Client mqttClient;
char registertopic[64];
char statustopic[64];
int ledslit=0;

void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status) {
    struct ip_info ipConfig;
    os_timer_disarm(&btnWiFiLinker);
    wifi_get_ip_info(STATION_IF, &ipConfig);
    int wifiStatus = wifi_station_get_connect_status();
	if(wifiStatus == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
        
    }
    os_timer_setfn(&btnWiFiLinker, (os_timer_func_t *)wifiConnectCb, NULL);
    os_timer_arm(&btnWiFiLinker, 2000, 0);
    
}

void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
    char temp[64];
    char temperature[128];
    os_sprintf(registertopic,"thebutton/cb/%s/register",system_get_chip_id());
    os_sprintf(statustopic,"thebutton/cb/%s/set",system_get_chip_id());

	MQTT_Subscribe(client, statustopic, 2);
    
    INFO("MQTT: Connected! subscribe to: %s\r\n", statustopic);
    INFO("MQTT: Connected! RegisterTopic to: %s\r\n", registertopic);
    os_sprintf(temp,"{\"register\": \"%s\"}\r\n", system_get_chip_id());
    MQTT_Publish(client, registertopic, temp, os_strlen(temp), 2, 0);
}

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}

void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len) {
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);
	MQTT_Client* client = (MQTT_Client*)args;
    int i;
    char *tmp_gpios, *status, *out,*temp, *clean;
    
	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;
	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;
    //Clean dataBuf
    clean = strstr(dataBuf,"{");
    INFO("Received on topic: %s ", topicBuf);
    INFO("Data: %s\n",clean);
    //PARSE MESSG
    /*
     ADDLED
     REMOVELED
     NOLED
     FLASHLED
     */
     if(os_strcmp(dataBuf,"ADDLED")==0) {
         ledslit+=1;
     }
    if(os_strcmp(dataBuf,"REMOVELED")==0) {
        ledslit-=1;
    }
    if(os_strcmp(dataBuf,"NOLED")==0) {
        ledslit=0;
    }
    if(os_strcmp(dataBuf,"FLASHLED")==0) {
        ledslit=8;
        
    }
    lightleds(ledslit);
    //Clean up.
    os_delay_us(1000);
    //delay(1);
    os_free(status);
    /*os_free(clean);
    os_free(topic);
    os_free(data);
    */
    //os_free(topicBuf);
    os_free(dataBuf);
}


void ICACHE_FLASH_ATTR init_mqtt(void) {
    char temp[128];
    MQTTCFG mqttcfg;
    mqttcfg.mqtt_host="mqtt.murf.se";
    mqttcfg.mqtt_port=8885;
    mqttcfg.mqtt_user="mqtt";
    mqttcfg.mqtt_pass="test";
    mqttcfg.mqtt_keepalive=120;
    mqttcfg.security = 1 // 1=ssl (max 1024) 0=nonssl
    
	MQTT_InitConnection(&mqttClient, mqttcfg.mqtt_host, mqttcfg.mqtt_port, mqttcfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	//MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
	
    
    //Timer to make sure the MQTTclient is connected
    os_timer_disarm(&btnWiFiLinker);
    os_timer_setfn(&btnWiFiLinker, (os_timer_func_t *)wifiConnectCb, NULL);
    os_timer_arm(&btnWiFiLinker, 2000, 0);
    
    //Set up the gpios for
    
    // Init timer to send DS18B20 Dat
    /*
    os_timer_disarm(&ds18b20_timer);
    os_timer_setfn(&ds18b20_timer, (os_timer_func_t *)timer_send_temperature, NULL);
    os_timer_arm(&ds18b20_timer, 1000*60, 1);
    */

    

}
