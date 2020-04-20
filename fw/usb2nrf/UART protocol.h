/*
 * UART_protocol.h
 *
 * Created: 14.04.2020 11:09:23
 *  Author: Mintytail
 */ 


#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_
#include "defines.h"
#include <stdint.h>
#include "../usb2nrf/UART functions.h"

typedef enum {
	eucOk = 0,
	eucGeneralFail = 0x80,
	eucBadVersion = 0x90,
	eucBadCommand = 0x91,
	eucMemoryError = 0x92,
	eucArgumentValidationError = 0x93,
	eucNotImplemented = 0x94,
} eUARTResponseCodes;

// packets from uart
#define HEADER_SIZE (1 + 1)
typedef union {
	uint8_t packageBuffer[(HEADER_SIZE + PAYLOAD_SIZE)];
	struct {
		eModemCommand command;
		uint8_t payloadSize;
		uint8_t payload[PAYLOAD_SIZE];
	} pkg;
} uPackage;

#endif /* UART_PROTOCOL_H_ */