/*
 * RF_statistics.c
 *
 * Created: 05.03.2020 21:54:49
 *  Author: Mintytail
 */ 

#include <stdint.h>
#ifndef UNIT_TESTING
	#include <avr/interrupt.h>
#endif
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"
#include "../usb2nrf/defines.h"
#include "../usb2nrf/nRF model.h"

nRF24L01 *rfTransiever;
fNRFCallback cNRF_DataTransmitted = NULL, cNRF_DataReceived = NULL, cNRF_TransmissionFailed = NULL;
uint16_t total_requests, ok_responses, error_responses, transaction_errors, ack_timeouts;

void nRF_init() {
	// nrf24l01
	// interrupt from pcint0 falling edge
	PCICR = (1 << PCIE0);
	// pin b0 change interrupt enable
	uint8_t wr = 0;
	PCMSK0 = (1 << PCINT0);
	rfTransiever = nRF24L01_init();
	rfTransiever->ss.port = &portTransiever; rfTransiever->ss.pin = PORTB1;
	rfTransiever->ce.port = &portTransiever; rfTransiever->ce.pin = PORTB2;
	rfTransiever->mosi.port = &portTransiever; rfTransiever->mosi.pin = PORTB3;
	rfTransiever->mosi.port = &portTransiever; rfTransiever->miso.pin = PORTB4;
	// turn on pull-up for miso, to increase spi speed, maybe
	// internal pull up resistor is 20k to 50k
	PORTB |= _BV(rfTransiever->miso.pin);
	rfTransiever->sck.port = &portTransiever; rfTransiever->sck.pin = PORTB5;
	nRF24L01_begin(rfTransiever);
	// 1Mbps, max power
	wr = 3 << RF_PWR; nRF24L01_write_register(rfTransiever, RF_SETUP, &wr, 1);
}

ISR(PCINT0_vect) {
	// pin change, but we need only falling  edge
	if (portTransiever & (1 << poTransiever_IRQ)) return;
	// The IRQ pin is activated when TX_DS IRQ, RX_DR IRQ or MAX_RT IRQ are set high
	// by the state machine in the STATUS register
	nRF24L01_update_status(rfTransiever);
	if (rfTransiever->status & _BV(TX_DS)) {
		// Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. If AUTO_ACK is activated,
		// this bit is set high only when ACK is received.
		if (cNRF_DataTransmitted) (*cNRF_DataTransmitted)();
		uint8_t data = _BV(TX_DS);
		nRF24L01_write_register(rfTransiever, STATUS, &data, 1);
	}
	if (rfTransiever->status & _BV(MAX_RT)) {
		// Maximum number of TX retransmits interrupt
		// If MAX_RT is asserted it must be cleared to enable further communication.
		if (cNRF_TransmissionFailed) (*cNRF_TransmissionFailed)();
		uint8_t data = _BV(MAX_RT);
		nRF24L01_write_register(rfTransiever, STATUS, &data, 1);
	}
	if (rfTransiever->status & _BV(RX_DR)) {
		// Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO
		// The RX_DR IRQ is asserted by a new packet arrival event.
		// The procedure for handling this interrupt should be:
		// 1) read payload through SPI,
		// 2) clear RX_DR IRQ,
		// 3) read FIFO_STATUS to check if there are more payloads available in RX FIFO,
		// 4) if there are more data in RX FIFO, repeat from step 1).
		do {
			if (cNRF_DataReceived) (cNRF_DataReceived)();
			uint8_t data = _BV(RX_DR);
			nRF24L01_write_register(rfTransiever, STATUS, &data, 1);
		} while (nRF24L01_data_received(rfTransiever));
	}

	//checkTransieverRXBuf(/*true*/);
}
