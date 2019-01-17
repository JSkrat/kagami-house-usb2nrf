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

typedef enum {
	usError,
	usStart,
	usSPackage,
} usState;

union uPackage {
	uint8_t packageBuffer[MACSize + 1 + 32];
	struct {
		uint8_t MAC[MACSize];
		uint8_t length;
		uint8_t data[32];
	} package;
	struct {
		uint8_t version;
		uint8_t packagesLost;
		uint8_t receiveQueueLength;
	};
};

/*typedef struct {
	uint8_t length;
	uint8_t data[];
} string;

extern const string m_test PROGMEM;*/


void parse(unsigned char b);
//void queue(char *str);



#endif /* UART_PARSER_H_ */