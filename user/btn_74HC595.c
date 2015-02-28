

/*
* Adaptation Murf
 */

#include "ets_sys.h"
#include "osapi.h"
#include "debug.h"
#include "mqtt.h"

#include "gpio.h"
#include "mem.h"
#include "btn_74HC595.h"


void ICACHE_FLASH_ATTR noleds() {
    //INFO("Set NoLEDS\n");
    int i=0;
    GPIO_OUTPUT_SET(DataPin, 0);
    for(i=0; i<=8; i++) {
        GPIO_OUTPUT_SET(ClockPin, 1);
        GPIO_OUTPUT_SET(ClockPin, 0);
    }
    GPIO_OUTPUT_SET(LatchPin, 1);
    //Reset pins to OFF
    GPIO_OUTPUT_SET(LatchPin, 0);
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(ClockPin, 0);
}

void ICACHE_FLASH_ATTR lightleds(int ledslit) {
    int i;
    noleds();
    //INFO("Set %d LEDS\n",ledslit);
    GPIO_OUTPUT_SET(DataPin, 1);
    for(i=1; i<=ledslit; i++) {
        GPIO_OUTPUT_SET(ClockPin, 1);
        GPIO_OUTPUT_SET(ClockPin, 0);
    }
    GPIO_OUTPUT_SET(LatchPin, 1);
    //Reset pins to OFF
    GPIO_OUTPUT_SET(LatchPin, 0);
    GPIO_OUTPUT_SET(DataPin, 0);
    GPIO_OUTPUT_SET(ClockPin, 0);

    
}
void ICACHE_FLASH_ATTR flashleds() {
    int y;
    for(y=0;y<=8;y++) {
        os_delay_us(100000);
        lightleds(y);
    }
    lightleds(0);
}
/* Im thinking.... I want a shiftout bitbangin function.
void ICACHE_FLASH_ATTR shiftOut(unsigned char inputData) {
    unsigned char i;					// Counter
    unsigned char tempShift;
    
    for(i=0; i<8; i++9) {		// Cycle 8 times (byte)
        tempShift = inputData & 0x80;
        
        if(tempShift == 0x80){ 			// Shift out Data MSB first
            PORTB |= (1 << dataPin);	// Set dataPin HIGH
        } else {
            PORTB &= ~(1 << dataPin);	// Set dataPin LOW
        }
        PORTB |= (1 << shiftPin);		// Set clockPin HIGH
        PORTB &= ~(1 << shiftPin);		// set clockPin LOW
        
        inputData = inputData << 1;		// Shift Data one bit left
    }
    
    PORTB |= (1 << latchPin);			// Enable storage -> output 7 seg
    PORTB &= ~(1 << latchPin);			// Disable storage
}
 */

