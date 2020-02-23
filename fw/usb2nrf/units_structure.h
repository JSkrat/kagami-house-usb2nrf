/*
 * units_structure.h
 *
 * Created: 09.02.2020 16:35:36
 *  Author: Mintytail
 */ 


#ifndef UNITS_STRUCTURE_H_
#define UNITS_STRUCTURE_H_
#include <stdbool.h>
#include "functions.h"

bool switches[5];
bool relays[2];
uint16_t analog_output;

extern const sUnit units[UNITS_LENGTH];


#endif /* UNITS_STRUCTURE_H_ */