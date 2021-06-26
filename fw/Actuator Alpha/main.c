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

static int adv = 0;
#define ADV_TRIGGER 2

int main(void)
{
	// disable comparator
	ACSR |= _BV(ACD);
	rf_init();
	functions_init(CRFUnits, unitsCount);
	DDRD |= _BV(PORTD7); // adv led
	DDRD &= ~_BV(PORTD6); // adv btn
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	
	while (1) {
		if (ADV_TRIGGER > adv) {
			if (PIND & _BV(PIND6)) adv++;
		} else {
			adv = 0;
			switchMode(emSearchMaster);
		}
		if (emSearchMaster == getRFMode()) {
			PORTD |= _BV(PORTD7);
		} else {
			PORTD &= ~_BV(PORTD7);
		}
		sleep_mode();
	}
}
