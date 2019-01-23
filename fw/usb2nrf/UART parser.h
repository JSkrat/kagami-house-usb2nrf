/*
 * UART_parser.h
 *
 * Created: 13.01.2019 16:25:31
 *  Author: J-sama
 */ 


#ifndef UART_PARSER_H_
#define UART_PARSER_H_
#include "defines.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "messages.h"

typedef enum {
	usError,
	usStart,
	usSPackage,
} usState;

union uPackage {
	uint8_t packageBuffer[MAC_SIZE + 1 + 32];
	struct {
		uint8_t MAC[MAC_SIZE];
		uint8_t length;
		uint8_t data[32];
	} package;
	struct {
		uint8_t version;
		uint8_t packagesLost;
		uint8_t receiveQueueLength;
	};
};

uint8_t sendBufferBegin;
uint8_t sendBufferEnd;
const string *lastCommand;

void parse(unsigned char b);
#define U_TRANSMIT_START UCSR0B |= (1 << UDRIE0)
void uQueueChar(const char c);
void uQueueString(const string *data);



#endif /* UART_PARSER_H_ */