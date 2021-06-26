/*
 * Black debug.c
 *
 * Created: 19.07.2020 13:32:28
 * Author : Mintytail
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "../KagamiCore/RF model.h"
#include "RF custom functions.h"
#include "../KagamiCore/RF functions.h"
//#include "../KagamiCore/UART parser.h"

int main(void)
{
	// disable comparator
	ACSR |= _BV(ACD);
	rf_init();
	functions_init(CRFUnits, unitsCount);
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
	while (1) {
		sleep_mode();
	}
}
