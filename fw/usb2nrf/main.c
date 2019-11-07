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
#include "defines.h"
#include "UART parser.h"
#include "RF parser.h"
#include "ui.h"
#include "my-stdlib.h"
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"

ISR(BADISR_vect, ISR_NAKED) {
	// nothing here
}

int main(void)
{
	/* hardware initialization */
	ui_init();
	u_init();
	rf_init();
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
	portLEDS &= ~(1 << poLED_D1);
    while (1) {
		ui_redraw();
		sleep_mode();
    }
}

