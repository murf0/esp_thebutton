#include "mqtt.h"

#define DataPin     14 //DS, DataPin
#define LatchPin    12 //ST_CP, LatchPin
#define ClockPin    13 //SH_CP, ClockPin

extern void ets_wdt_enable(void);
extern void ets_wdt_disable(void);
extern void wdt_feed(void);

void ICACHE_FLASH_ATTR lightleds(int ledslit);
void ICACHE_FLASH_ATTR flashleds(int times);
void ICACHE_FLASH_ATTR noleds();
void ICACHE_FLASH_ATTR flashleds_timed(int times);
