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
#define uPPProtoVer 0

// modem commands
enum eModemCommand {
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
	
	mcTransmit = 0x7F,
	
	mcAckFromRF = 0xCC,
	mcReceiveFromRF = 0xEE,
};

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
		enum eModemCommand command;
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

void nListen();


#endif /* UART_PARSER_H_ */