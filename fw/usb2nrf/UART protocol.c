/*
 * UART_protocol.c
 *
 * Created: 19.04.2020 20:57:19
 *  Author: Mintytail
 */ 

#include "UART protocol.h"
#include "UART functions.h"
#include "string.h"
#include <stddef.h>

void UARTGenerateResponse(const uPackage const *request, uPackage *response) {
	response->pkg.command = request->pkg.command | 0x80;
	response->pkg.payloadSize = 1;
	// search for the requested command
	for (uint8_t i = 0; i < UART_FUNCTIONS_NUMBER; i++) {
		eModemCommand commandToCheck = pgm_read_byte(UARTFunctions[i].command);
		if (request->pkg.command == commandToCheck) {
			fUARTFunction f = pgm_read_ptr(UARTFunctions[i].function);
			if (NULL == f) {
				// generate response "not implemented"
				response->pkg.payload[0] = eucNotImplemented;
			} else {
				scString sUARTRequest = {.length = request->pkg.payloadSize, .data = &request->pkg.payload[0]};
				sString sUARTResponse = {.length = 0, .data = &response->pkg.payload[1]};
				response->pkg.payload[0] = f(&sUARTRequest, &sUARTResponse);
				response->pkg.payloadSize = sUARTResponse.length + 1;
			}
			return;
		}
	}
	// command not found, generate error response about it
	response->pkg.payload[0] = eucBadCommand;
}