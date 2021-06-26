/*
 * usb2nrf.c
 *
 * Created: 24.12.2018 22:54:22
 * Author : J-sama
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
//#include "defines.h"
#include "../usb2nrf/UART parser.h"
#include "../usb2nrf/RF model.h"
//#include "my-stdlib.h"
//#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
//#include "avr-nrf24l01-master/src/nrf24l01.h"

ISR(BADISR_vect, ISR_NAKED) {
	// nothing here
}

int main(void)
{
	/* hardware initialization */
#if BT_MASTER == BUILD_TYPE
	u_init();
#endif
	rf_init();
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
    while (1) {
		sleep_mode();
    }
}

