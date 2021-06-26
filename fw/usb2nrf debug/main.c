/*
 * Actuator Alpha.c
 *
 * Created: 04.07.2020 22:09:39
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
	rf_init();
	functions_init(RFUnits, unitsCount);
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
	while (1) {
		sleep_mode();
	}
}
