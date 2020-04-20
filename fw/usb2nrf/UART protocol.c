/*
 * UART_protocol.c
 *
 * Created: 19.04.2020 20:57:19
 *  Author: Mintytail
 */ 

#include "UART protocol.h"
#include "UART functions.h"
#include "sstring.h"
#include <stddef.h>
#include <stdbool.h>
#include "messages.h"
#include "ui.h"

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

static usState state;
static uint8_t position = 0;
static uPackage reqBuffer;


void UARTGenerateResponse(const uPackage *request, uPackage *response) {
	response->pkg.protocolVersion = uPPProtoVer;
	response->pkg.command = request->pkg.command | 0x80;
	response->pkg.payloadSize = 1;
	if (uPPProtoVer != request->pkg.protocolVersion) {
		response->pkg.payload[0] = eucBadVersion;
		return;
	}
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

void UARTBeginTransaction() {
	state = usProtoVer;
	ui_subsystem_str(ui_s_uart_packet_fsm, &m_usProtoVer, true);
}

bool UARTProcessNextByte(uint8_t b, uPackage *response) {
	switch (state) {
		case usEnd:
		case usError: {
			//ui_subsystem_str(ui_s_uart_packet_fsm, &m_usError, true);
			break;
		}
		case usProtoVer: {
			if (uPPProtoVer == b) {
				state = usCommand;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usCommand, true);
				} else {
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usErrorProto, true);
			}
			break;
		}
		case usCommand: {
			if (0x80 & b) {
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usErrorCmd, true);
				} else {
				reqBuffer.pkg.command = b;
				state = usPDataLength;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usPDataLength, true);
			}
			break;
		}
		case usPDataLength: {
			if (PAYLOAD_SIZE + MAC_SIZE <= b) {
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usErrorLength, true);
				break;
			}
			reqBuffer.pkg.payloadSize = b;
			position = 0;
			if (0 == b) {
				// there won't be any bytes from that package anymore
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usEnd, true);
				UARTGenerateResponse(&reqBuffer, response);
				return true;
			} else {
				state = usPData;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usPData, true);
			}
			break;
		}
		case usPData: {
			reqBuffer.pkg.payload[position++] = b;
			if (reqBuffer.pkg.payloadSize <= position) {
				// that's all payload, no bytes from that package left
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usEnd, true);
				UARTGenerateResponse(&reqBuffer, response);
				return true;
			}
			break;
		}
	}
	return false;
}

void UARTProtocolInit() {
	state = usError;
	ui_subsystem_str(ui_s_uart_packet_fsm, &m_usNotInit, true);
}
