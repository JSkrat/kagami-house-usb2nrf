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
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"

ISR(BADISR_vect, ISR_NAKED) {
	// nothing here
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
	
	// nrf24l01
	// interrupt from pcint0 falling edge
	PCICR = (1 << PCIE0);
	// pin b0 change interrupt enable
	PCMSK0 = (1 << PCINT0);
	rfTransiever = nRF24L01_init();
	rfTransiever->ss.port = &portTransiever; rfTransiever->ss.pin = PORTB1;
	rfTransiever->ce.port = &portTransiever; rfTransiever->ce.pin = PORTB2;
	rfTransiever->mosi.port = &portTransiever; rfTransiever->mosi.pin = PORTB3;
	rfTransiever->mosi.port = &portTransiever; rfTransiever->miso.pin = PORTB4;
	rfTransiever->sck.port = &portTransiever; rfTransiever->sck.pin = PORTB5;
	nRF24L01_begin(rfTransiever);
	nListen();
	
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
	lcdLocate(0, 0);
	lcdPrint(&m_hello, normal);
    while (1) {
		lcdLocate(1, 0);
		lcdPrint(&m_sendQueueSize, normal);
		lcdRamPrint(SStr(sendBufferEnd - sendBufferBegin), normal);
		
		lcdLocate(2, 0);
		lcdPrint(&m_last_command, normal);
		lcdPrint(lastCommand, normal);
		sleep_mode();
    }
}

