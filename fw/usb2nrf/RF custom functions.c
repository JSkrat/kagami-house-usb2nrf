/*
 * RF_custom_functions.c
 *
 * Created: 23.05.2020 16:53:54
 *  Author: Mintytail
 */ 

#include "RF custom functions.h"
#include "RF functions.h"
#include "RF protocol.h"
#include <avr/pgmspace.h>

uint8_t readPin(const uint8_t function, const scString *request, sString *response) {
	return ercNotImplemented;
}

uint8_t writePin(const uint8_t function, const scString *request, sString *response) {
	return ercNotImplemented;
}

const PROGMEM tRFCodeFunctionItem U1Functions[fU1Count] = {
	{
		.functionCode = 0x10, .type.fields.input = edtNone, .type.fields.output = edtBool,
		.function = &readPin
	},
	{
		.functionCode = 0x11, .type.fields.input = edtBool, .type.fields.output = edtNone,
		.function = &writePin
	},
};

const PROGMEM tUnit RFUnits[unitsCount] = {
	{ .length = fU1Count, .functions = U1Functions },
};
