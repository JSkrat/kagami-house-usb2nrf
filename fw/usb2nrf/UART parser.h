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

// packet prefixes
// uart to nrf
#define uPPU2N 0xFF
// data packet nrf to uart
#define uPPN2U 0xEE
// no ack after send uart to nrf
#define uPPNTO 0xDD
// ack after send uart to nrf
#define uPPNACK 0xCC

// modem commands
#define mcStatus 0
#define mcSetChannel 0x10
#define mcSetTXPower 0x11

typedef enum {
	usError,
	usStart,
	usPAddressTo,
	usPDataLength,
	usPData,
} usState;
usState state;

// packets from uart
union uPackage {
	uint8_t packageBuffer[(MAC_SIZE + 1 + 32)];
	struct {
		uint8_t address[MAC_SIZE];
		nRF24L01Message msg;
	} rfMsg;
};
union uPackage packageBuffer;

uint8_t sendBufferBegin;
uint8_t sendBufferEnd;

nRF24L01 *rfTransiever;

uint16_t badRFPackets;

void parse(unsigned char b);
#define U_TRANSMIT_START UCSR0B |= (1 << UDRIE0)
void uQueueChar(const uint8_t c);
void uQueueString(const string *data);

void nListen();


#endif /* UART_PARSER_H_ */