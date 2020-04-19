/*
 * UART_protocol.h
 *
 * Created: 14.04.2020 11:09:23
 *  Author: Mintytail
 */ 


#ifndef UART_PROTOCOL_H_
#define UART_PROTOCOL_H_


typedef enum {
	eucOk = 0,
	eucGeneralFail = 0x80,
	eucArgumentValidationError = 0x81,
} eUARTResponseCodes;


#endif /* UART_PROTOCOL_H_ */