#ifndef BTN_MQTT_H
#define BTN_MQTT_H

typedef struct{
    uint8_t mqtt_host[64];
    uint32_t mqtt_port;
    uint8_t mqtt_user[32];
    uint8_t mqtt_pass[32];
    uint32_t mqtt_keepalive;
    uint8_t security;
} MQTTCFG;

void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status);
void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args);
void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args);
void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args);
void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);
void ICACHE_FLASH_ATTR init_mqtt(void);

#endif
