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
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"
#include <string.h>

usState state = usStart;
uint8_t position = 0;
uint8_t dataLength = 0;
union uPackage packageBuffer;

const string *lastCommand = &m_c_none;

// +1 for make sure begin and end will be equal only at empty buffer
#define SEND_BUFFER_SIZE (1 + MAC_SIZE + PAYLOAD_SIZE + 1)
uint8_t sendBuffer[SEND_BUFFER_SIZE];
/// sendBufferBegin points to next charachter to send
/// sendBufferEnd points to next empty space in buffer
uint8_t sendBufferBegin = 0, sendBufferEnd = 0;

nRF24L01 *rfTransiever;
const uint8_t modemAddress[5] = { 0x00, 0x00, 0x00, 0x00, 0x01 };
uint8_t badRFPackets = 0;

void uQueueChar(const unsigned char c) {
	/// TODO check "buffer full" case and show on screen and turn on red for some time
	// first put charachter
	sendBuffer[sendBufferEnd] = c;
	// then move pointer so it won't send garbage
	sendBufferEnd++;
	if (SEND_BUFFER_SIZE <= sendBufferEnd) sendBufferEnd = 0;	
}

void uQueueBytes(const unsigned char *bytes, const uint8_t number) {
	for (uint8_t p = 0; p < number; p++) {
		uQueueChar(bytes[p]);
	}
}

void uQueueString(const string *data) {
	for (uint8_t p = 0; p < data->length; p++) {
		uQueueChar(data->data[p]);
	}
}

void checkTransieverRXBuf() {
	while (nRF24L01_data_received(rfTransiever)) {
		nRF24L01Message msg;
		nRF24L01_read_received_data(rfTransiever, &msg);
		// assemble packet for uart
		// and discard everything with payload shorter than 5 bytes
		if (MAC_SIZE > msg.length) {
			badRFPackets++;
			continue;
		}
		/// TODO check if buffer has enough space for packet
		uQueueChar(uPacketPrefix);
		uQueueBytes(msg.data, MAC_SIZE);
		uQueueChar(msg.length - MAC_SIZE);
		uQueueBytes(msg.data+MAC_SIZE, msg.length - MAC_SIZE);
	}
	nListen();
}

void nListen() {
	nRF24L01_listen(rfTransiever, 0, (uint8_t*) modemAddress);
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
		checkTransieverRXBuf();
		return;
	}
	UDR0 = sendBuffer[sendBufferBegin++];
	if (SEND_BUFFER_SIZE <= sendBufferBegin) sendBufferBegin = 0;
}

ISR(PCINT0_vect) {
	// pin change, but we need only falling  edge
	if (portTransiever & (1 << poTransiever_IRQ)) return;
	checkTransieverRXBuf();
}

void parse(unsigned char b)
{
	switch (state) {
		case usError: {
			break;
		}
		case usStart: {
			if (uPacketPrefix == b) {
				state = usPAddressTo;
				position = 0;
			} else {
				state = usError;
			}
			break;
		}
		case usPAddressTo: {
			packageBuffer.rfMsg.address[position++] = b;
			if (MAC_SIZE <= position) {
				state = usPDataLength;
			}
			break;
		}
		case usPDataLength: {
			packageBuffer.rfMsg.msg.length = b + MAC_SIZE;
			memcpy(packageBuffer.rfMsg.msg.data, modemAddress, MAC_SIZE);
			position = MAC_SIZE;
			if (0 == b) {
				state = usStart;
				// send packet to nrf here
			} else {
				state = usPData;
			}
			break;
		}
		case usPData: {
			packageBuffer.rfMsg.msg.data[position++] = b;
			if (packageBuffer.rfMsg.msg.length <= position) {
				state = usStart;
				// send packet to nrf here
			}
			break;
		}
	}
}
