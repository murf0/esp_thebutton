#include "mqtt.h"

#define DataPin     14 //DS, DataPin
#define LatchPin    12 //ST_CP, LatchPin
#define ClockPin    13 //SH_CP, ClockPin

void ICACHE_FLASH_ATTR lightleds(int ledslit);
void ICACHE_FLASH_ATTR flashleds(int times);
void ICACHE_FLASH_ATTR noleds();
void ICACHE_FLASH_ATTR flashleds_timed(int times);