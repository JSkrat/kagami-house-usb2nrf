/*
 * UART_parser.c
 *
 * Created: 13.01.2019 16:25:04
 *  Author: J-sama
 */ 
#include "UART parser.h"
#include "defines.h"
#include <stdint.h>
#include "messages.h"
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include "RF parser.h"
#include "ui.h"

void uSendPacket(union uPackage *packet);

static usState state;
static uint8_t position = 0;
static union uPackage reqBuffer;
static bool escActive;

// +1 for make sure begin and end will be equal only at empty buffer
#define SEND_BUFFER_SIZE ((1 + MAC_SIZE + PAYLOAD_SIZE + 1)*2)
uint8_t sendBuffer[SEND_BUFFER_SIZE];
/// sendBufferBegin points to next character to send
/// sendBufferEnd points to next empty space in buffer
uint8_t sendBufferBegin = 0, sendBufferEnd = 0;

const uint8_t modemAddress[5] = { 0xA5, 0x42, 0x42, 0x42, 0x02 };
uint8_t replyAddress[MAC_SIZE];
uint16_t badRFPackets = 0;
uint16_t rfTimeouts = 0;
uint16_t rfPacketsSent = 0;

void u_init() {
	DDRD = (1 << PORTD1);
	// USART
	#define MYUBRR (F_CPU/8/BAUDRATE-1)
	UBRR0H = (unsigned char) (MYUBRR>>8);
	UBRR0L = (unsigned char) (MYUBRR);
	// double speed, usart enable
	UCSR0A = (1 << U2X0);
	// enable rx and interrupts
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0);
	// asynchronous, 8N1
	UCSR0C = (0b00 << UMSEL00) | (0b11 << UCSZ00) | (0b00 << UPM00) | (0 << USBS0);
	
	state = usError;
}

void uQueueChar(const uint8_t c) {
	/// TODO check "buffer full" case and show on screen and turn on red for some time
	// first put character
	sendBuffer[sendBufferEnd] = c;
	// then move pointer so it won't send garbage
	sendBufferEnd++;
	if (SEND_BUFFER_SIZE <= sendBufferEnd) sendBufferEnd = 0;	
}

void uQueueBytes(const uint8_t *bytes, const uint8_t number) {
	for (uint8_t p = 0; p < number; p++) {
		uQueueChar(bytes[p]);
	}
}

void uQueueString(const string *data) {
	for (uint8_t p = 0; p < data->length; p++) {
		uQueueChar(data->data[p]);
	}
}

ISR(USART_RX_vect) {
	// on not empty receive buffer
	// error flags for current UDR0, must be read before reading UDR0
	// UCSR0A: FE0 frame error (stop bit is not 1), DOR0 data overrun, UPE0 parity error
	parse(UDR0);
}

ISR(USART_UDRE_vect) {
	/// when data register is empty and ready to send another byte
	// check if buffer is empty
	if (sendBufferBegin == sendBufferEnd) {
		UCSR0B &= ~(1 << UDRIE0);
		// it's a little overkill, to check rf transiever every time uart buffer empty
		//checkTransieverRXBuf();
		return;
	}
	UDR0 = sendBuffer[sendBufferBegin++];
	if (SEND_BUFFER_SIZE <= sendBufferBegin) sendBufferBegin = 0;
}

void processPacket() {
	union uPackage respBuffer;
	switch (reqBuffer.pkg.command) {
		case mcStatus: {
			respBuffer.pkg.payloadSize = 8;
			respBuffer.pkg.payload[0] = 0; // protocol version
			respBuffer.pkg.payload[1] = nRF24L01_get_status(rfTransiever);
			*((uint16_t*) (respBuffer.pkg.payload+2)) = rfPacketsSent;
			*((uint16_t*) (respBuffer.pkg.payload+4)) = rfTimeouts;
			*((uint16_t*) (respBuffer.pkg.payload+6)) = badRFPackets;
			break;
		}
		case mcSetChannel: {
			nRF24L01_write_register(rfTransiever, RF_CH, &(reqBuffer.pkg.payload[1]), 1);
			respBuffer.pkg.payloadSize = 1;
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcSetBitRate: {
			uint8_t rf_setup; 
			nRF24L01_read_register(rfTransiever, RF_SETUP, &rf_setup, 1);
			rf_setup &= ~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH));
			if (reqBuffer.pkg.payload[1] & 0b00000010) rf_setup |= (1 << RF_DR_LOW);
			if (reqBuffer.pkg.payload[1] & 0b00000001) rf_setup |= (1 << RF_DR_HIGH);
			nRF24L01_write_register(rfTransiever, RF_SETUP, &rf_setup, 1);
			respBuffer.pkg.payloadSize = 1;
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcSetTXPower: {
			uint8_t rf_setup;
			nRF24L01_read_register(rfTransiever, RF_SETUP, &rf_setup, 1);
			rf_setup &= ~(0b00000011 << RF_PWR);
			rf_setup |= (reqBuffer.pkg.payload[1] & 0b00000011) << RF_PWR;
			nRF24L01_write_register(rfTransiever, RF_SETUP, &rf_setup, 1);
			respBuffer.pkg.payloadSize = 1;
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcClearTX: {
			nRF24L01_flush_transmit_message(rfTransiever);
			respBuffer.pkg.payloadSize = 1;
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcListen: {
			nListen((t_address *) reqBuffer.pkg.payload);
			respBuffer.pkg.payloadSize = 1;
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcTransmit: {
			respBuffer.pkg.payloadSize = 1;
			if (MAC_SIZE > reqBuffer.pkg.payloadSize) {
				respBuffer.pkg.payload[0] = 1;
				break;
			}
			checkTransieverRXBuf();
			_delay_us(10);
			tRfPacket packet;
			memcpy(packet.address, reqBuffer.pkg.payload, MAC_SIZE);
			packet.msg.length = reqBuffer.pkg.payloadSize - MAC_SIZE;
			if (packet.msg.length) {
				memcpy(packet.msg.data, reqBuffer.pkg.payload + MAC_SIZE, packet.msg.length);
			}
			transmitPacket(&packet);
			respBuffer.pkg.payload[0] = 0;
			break;
		}
		case mcSetAutoRetransmitDelay:
		case mcSetAutoRetransmitCount:
		case mcClearRX:
		case mcAckFromRF:
		case mcReceiveFromRF:
			break;
	}
	// lets queue the answer
	uSendPacket(&respBuffer);
}

void uSendPacket(union uPackage *packet) {
	uQueueChar(0xC0);
	uQueueChar(uPPProtoVer);
	for (int8_t i = 0; i < packet->pkg.payloadSize + HEADER_SIZE; i++) {
		char c = packet->packageBuffer[i];
		if (0 == i) c |= 0x80;
		if (0xC0 == c) {
			uQueueChar(0xDB);
			c = 0xDC;
			} else if (0xDB == c) {
			uQueueChar(0xDB);
			c = 0xDD;
		}
		uQueueChar(c);
	}
	U_TRANSMIT_START;
}

/** \brief Parse commands from USART
 */
void parse(unsigned char b)
{
	// byte stuffing (or escaping) first
	switch (b) {
		case 0xC0: {
			state = usProtoVer;
			ui_subsystem_str(ui_s_uart_packet_fsm, &m_usProtoVer, true);
			escActive = false;
			return;
		}
		case 0xDB: {
			escActive = true;
			return;
		}
		case 0xDC: {
			if (escActive) {
				b = 0xC0;
				escActive = false;
			}
			break;
		}
		case 0xDD: {
			if (escActive) {
				b = 0xDB;
				escActive = false;
			}
			break;
		}
	}
	// now parsing UART frame
	switch (state) {
		case usEnd:
		case usError: {
			ui_subsystem_str(ui_s_uart_packet_fsm, &m_usError, true);
			break;
		}
		case usProtoVer: {
			if (uPPProtoVer == b) {
				state = usCommand;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usCommand, true);
			} else {
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usError, true);
			}
			break;
		}
		case usCommand: {
			if (0x80 & b) {
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usError, true);
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
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usError, true);
				break;
			}
			reqBuffer.pkg.payloadSize = b;
			position = 0;
			if (0 == b) {
				// there won't be any bytes from that package anymore
				state = usError;
				ui_subsystem_str(ui_s_uart_packet_fsm, &m_usEnd, true);
				processPacket();
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
				processPacket();
			}
			break;
		}
	}
}
