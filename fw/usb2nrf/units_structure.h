/*
 * units_structure.h
 *
 * Created: 09.02.2020 16:35:36
 *  Author: Mintytail
 */ 


#ifndef UNITS_STRUCTURE_H_
#define UNITS_STRUCTURE_H_
#include <stdbool.h>
#include <stdint.h>
#include "protocol.h"

#ifndef UNIT_TESTING
	#include <avr/pgmspace.h>
#else

#endif

bool switches[5];
bool relays[2];
uint16_t analog_output;

#define UNITS_LENGTH 6
extern const sUnit PROGMEM units[UNITS_LENGTH];
//#define UNITS_LENGTH sizeof(units)/sizeof(sUnit)

#endif /* UNITS_STRUCTURE_H_ */