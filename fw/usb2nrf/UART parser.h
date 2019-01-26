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
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"

#define uPacketPrefix 0xFF

typedef enum {
	usError,
	usStart,
	usPAddressTo,
	usPDataLength,
	usPData,
} usState;

union uPackage {
	uint8_t packageBuffer[(MAC_SIZE + 1 + 32)*8];
	struct {
		uint8_t address[MAC_SIZE];
		nRF24L01Message msg;
	} rfMsg;
};

uint8_t sendBufferBegin;
uint8_t sendBufferEnd;
const string *lastCommand;

nRF24L01 *rfTransiever;

uint8_t badRFPackets;

void parse(unsigned char b);
#define U_TRANSMIT_START UCSR0B |= (1 << UDRIE0)
void uQueueChar(const unsigned char c);
void uQueueString(const string *data);

void nListen();


#endif /* UART_PARSER_H_ */