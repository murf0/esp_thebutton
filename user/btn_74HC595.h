#include "mqtt.h"
void ICACHE_FLASH_ATTR initShiftIO(void);
void ICACHE_FLASH_ATTR lightleds(int ledslit);
void ICACHE_FLASH_ATTR flashleds();
void ICACHE_FLASH_ATTR BTN_Task(os_event_t *e);