/*
 * UART_parser.c
 *
 * Created: 13.01.2019 16:25:04
 *  Author: J-sama
 */ 
#include "UART parser.h"
#include "defines.h"
#include <stdint.h>

usState state = usStart;
uint8_t position = 0;
uint8_t dataLength = 0;
union uPackage packageBuffer;

//const string m_test PROGMEM = {2, {255, 255, 255, 255}};

void parse(unsigned char b)
{
	switch (state) {
		case usError: {
			break;
		}
		case usStart: {
			switch (b) {
				case '?': {
					break;
				}
				case 'R': {
					break;
				}
				case 'S': {
					state = usSPackage;
					position = 0;
					break;
				}
			}
			break;
		}
		case usSPackage: {
			packageBuffer.packageBuffer[position++] = b;
			if (MACSize+1 == position) {
				dataLength = b;
			} else if (MACSize+1 < position) {
				if (dataLength < position - (MACSize+1)) {
					state = usStart;
					// event to send to transiever
				}
			}
			break;
		}
	}
}
