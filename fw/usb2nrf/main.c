/*
 * usb2nrf.c
 *
 * Created: 24.12.2018 22:54:22
 * Author : J-sama
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include "defines.h"
#include "1202_lib.h"
#include "messages.h"
#include "UART parser.h"
#include "my-stdlib.h"

ISR(BADISR_vect, ISR_NAKED) {
	// nothing here
}

ISR(USART_RX_vect) {
	// on not empty receive buffer
	// error flags for current UDR0, must be read before reading UDR0
	// UCSR0A: FE0 frame error (stop bit is not 1), DOR0 data overrun, UPE0 parity error
	parse(UDR0);
}


int main(void)
{
	/* hardware initialization */
	// компаратор нафиг
	ACSR = (1 << ACD);
	
	// pins
	DDRC = (1 << poLCD_CLK) | (1 << poLCD_CS) | (1 << poLCD_DATA) | (1 << poLCD_RST);
	DDRD = (1 << poLED_GREEN) | (1 << poLED_YELLOW) | (1 << poLED_RED) | (1 << PORTD1);
	portLEDS |= (1 << poLED_RED);
	
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
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	// lcd
	lcdInit();
	
	portLEDS |= (1 << poLED_YELLOW);
	portLEDS &= ~(1 << poLED_RED);
	sei();
	uQueueChar('?');
	uQueueChar(0x55);
	uQueueChar('2');
	U_TRANSMIT_START;
	
	portLEDS |= (1 << poLED_GREEN);
	portLEDS &= ~(1 << poLED_YELLOW);
	//portLEDS = m_test.length;
	lcdClear();
	lcdLocate(1, 0);
	lcdPrint(&m_hello, normal);
    while (1) {
		lcdLocate(0, 0);
		lcdPrint(&m_sendQueueSize, normal);
		lcdRamPrint(SStr(sendBufferEnd - sendBufferBegin), normal);
		
		lcdLocate(2, 0);
		lcdPrint(&m_last_command, normal);
		lcdPrint(lastCommand, normal);
		sleep_mode();
    }
}

