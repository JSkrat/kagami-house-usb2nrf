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

void uQueueChar(const char c) {
	// first put charachter
	sendBuffer[sendBufferEnd] = c;
	// then move pointer so it won't send garbage
	sendBufferEnd++;
	if (SEND_BUFFER_SIZE <= sendBufferEnd) sendBufferEnd = 0;	
}

void uQueueString(const string *data) {
	/// TODO check "buffer full" case and show on screen
	for (uint8_t p = 0; p < data->length; p++) {
		uQueueChar(data->data[p]);
	}
}

ISR(USART_UDRE_vect) {
	/// when data register is empty and ready to send another byte
	// check if buffer is empty
	if (sendBufferBegin == sendBufferEnd) {
		UCSR0B &= ~(1 << UDRIE0);
		return;
	}
	UDR0 = sendBuffer[sendBufferBegin++];
	if (SEND_BUFFER_SIZE <= sendBufferBegin) sendBufferBegin = 0;
}

void parse(unsigned char b)
{
	switch (state) {
		case usError: {
			break;
		}
		case usStart: {
			switch (b) {
				case '?': {
					lastCommand = &m_c_status;
					// protocol version
					uQueueChar(0);
					// packages lost since last request
					uQueueChar(0);
					// packages in receive buffer
					uQueueChar(0);
					
					U_TRANSMIT_START;
					break;
				}
				case 'R': {
					lastCommand = &m_c_receive;
					break;
				}
				case 'S': {
					lastCommand = &m_c_send;
					state = usSPackage;
					position = 0;
					break;
				}
			}
			break;
		}
		case usSPackage: {
			packageBuffer.packageBuffer[position++] = b;
			if (MAC_SIZE+1 == position) {
				dataLength = b;
			} else if (MAC_SIZE+1 < position) {
				if (dataLength < position - (MAC_SIZE+1)) {
					state = usStart;
					// event to send to transiever
				}
			}
			break;
		}
	}
}
