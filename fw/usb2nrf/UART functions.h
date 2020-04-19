/*
 * UART_functions.h
 *
 * Created: 13.04.2020 22:08:33
 *  Author: Mintytail
 */ 


#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include "string.h"
#include <avr/pgmspace.h>


typedef uint8_t (*fUARTFunction)(const scString*, sString*);

// modem commands
typedef enum {
	mcStatus = 0,
	mcAddresses = 1,
	
	mcSetChannel = 0x10,
	mcSetTXPower = 0x11,
	mcSetBitRate = 0x12,
	mcSetAutoRetransmitDelay = 0x13,
	mcSetAutoRetransmitCount = 0x14,
	
	mcClearTX = 0x20,
	mcClearRX = 0x21,
	
	mcListen = 0x30,
	
	mcSetMode = 0x40,
	mcSetListenAddress = 0x41,
	
	mcReadRFBuffer = 0x50,
	
	mcTransmit = 0x7F,
	
	mcAckFromRF = 0xCC,
	mcReceiveFromRF = 0xEE,
} eModemCommand;


typedef struct {
	eModemCommand command;
	fUARTFunction function;
} tUARTCommandItem;

extern const PROGMEM tUARTCommandItem UARTFunctions[];

#endif /* UART_FUNCTIONS_H_ */