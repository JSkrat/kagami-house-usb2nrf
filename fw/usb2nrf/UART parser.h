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
#ifndef UNIT_TESTING
    #include <avr/pgmspace.h>
#endif
#include "messages.h"
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"
#include "../usb2nrf/UART functions.h"

// packet prefixes
#define uPPProtoVer 0

typedef enum {
	usError,
	usProtoVer,
	usCommand,
	usPDataLength,
	usPData,
	usEnd,
} usState;

// packets from uart
#define HEADER_SIZE (1 + 1)
union uPackage {
	uint8_t packageBuffer[(HEADER_SIZE + PAYLOAD_SIZE)];
	struct {
		eModemCommand command;
		uint8_t payloadSize;
		uint8_t payload[PAYLOAD_SIZE];
	} pkg;
} ;
//union uPackage packageBuffer;

//uint8_t sendBufferBegin;
//uint8_t sendBufferEnd;

//uint16_t badRFPackets;

void u_init();
void parse(unsigned char b);
#define U_TRANSMIT_START UCSR0B |= (1 << UDRIE0)
void uQueueChar(const uint8_t c);
void uQueueString(const string *data);
void uSendPacket(union uPackage *packet);


#endif /* UART_PARSER_H_ */
