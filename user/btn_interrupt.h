#define BTNGPIO                     0

static ETSTimer btnDebounceTimer,btnResetTimer;


void ICACHE_FLASH_ATTR init_interupt(void);
void ICACHE_FLASH_ATTR btnDebounce();
void ICACHE_FLASH_ATTR BTN_Task(os_event_t *e);