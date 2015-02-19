#define BTNGPIO                     0
#define BTN_TASK_PRIO        		1
#define BTN_TASK_QUEUE_SIZE         4

static ETSTimer btnDebounceTimer,btnResetTimer;
os_event_t btn_procTaskQueue[BTN_TASK_QUEUE_SIZE];

void ICACHE_FLASH_ATTR init_interupt(void);
void ICACHE_FLASH_ATTR btnDebounce();
void ICACHE_FLASH_ATTR BTN_Task(os_event_t *e);