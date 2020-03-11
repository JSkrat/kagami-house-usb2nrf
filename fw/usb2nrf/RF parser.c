/*
 * RF_parser.c
 *
 * Created: 03.11.2019 19:17:51
 *  Author: Mintytail
 */
#include "../usb2nrf/RF parser.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"
#include "../usb2nrf/UART parser.h"
#ifndef UNIT_TESTING
    #include <avr/interrupt.h>
#endif
#include <string.h>
#include "../usb2nrf/protocol.h"
#include "RF statistics.h"

void checkTransieverRXBuf(/*const bool listenAfterwards*/);
void parseRFPacket(tRfPacket *pkg);
void dataTransmitted();
void dataReceived();
void transmissionFailed();
void responseTimeoutEvent();
void msEvent();

nRF24L01 *rfTransiever;
eRFMode RFMode;
t_address ListenAddress;
int responseTimeout = -1; // negative value means it is disabled, event triggered when it becomes disabled

#ifndef UNIT_TESTING
ISR(PCINT0_vect) {
	// pin change, but we need only falling  edge
	if (portTransiever & (1 << poTransiever_IRQ)) return;
	// The IRQ pin is activated when TX_DS IRQ, RX_DR IRQ or MAX_RT IRQ are set high 
	// by the state machine in the STATUS register
	nRF24L01_update_status(rfTransiever);
	if (rfTransiever->status & _BV(TX_DS)) {
		// Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. If AUTO_ACK is acti-
		// vated, this bit is set high only when ACK is received.
		dataTransmitted();
		uint8_t data = _BV(TX_DS);
		nRF24L01_write_register(rfTransiever, STATUS, &data, 1);
	}
	if (rfTransiever->status & _BV(MAX_RT)) {
		// Maximum number of TX retransmits interrupt
		// If MAX_RT is asserted it must be cleared to enable further communication.
		transmissionFailed();
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
			dataReceived();
			uint8_t data = _BV(RX_DR);
			nRF24L01_write_register(rfTransiever, STATUS, &data, 1);
		} while (nRF24L01_data_received(rfTransiever));
	}

	//checkTransieverRXBuf(/*true*/);
}

ISR(TIMER0_COMPA_vect) {
	// this is being called every millisecond
	msEvent();
}
#endif

void rf_init() {
#ifndef UNIT_TESTING
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
	// now let's setup timer
	TCCR0A = (1 << WGM01) | (0 << WGM00); // CTC mode
	TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << WGM02); // clk_io/64
	OCR0A = 250; // compare match interrupt every millisecond
	TIMSK0 = (1 << OCIE0A);
#endif
	RFMode = rmIdle;
	protocolInit();
	// if we init in slave mode, we need to send "turn on" packet to master and make sure master received it
	//nListen();
	total_requests = 0;
	ok_responses = 0;
	error_responses = 0;
	transaction_errors = 0;
	ack_timeouts = 0;
}

/**
 * create ack response with given address and garbage ack code in the given pointer
 * !! ack code should be filled in after creation manually !!
 * @param packet where to create response packet
 * @param address ack source address
 */
void createUAckResponse(union uPackage *packet, uint8_t *address) {
	packet->pkg.command = mcAckFromRF;
	packet->pkg.payloadSize = MAC_SIZE + 1;
	memcpy(&(packet->pkg.payload), address, MAC_SIZE);
	packet->pkg.payload[MAC_SIZE] = 0;
}

void dataReceived() {
	// event, one per received packet
	tRfPacket request;
	union uPackage uartPacket;
	nRF24L01_read_received_data(rfTransiever, &(request.msg));
	// assemble packet for uart
	/// TODO check if buffer has enough space for packet
	// read pipe 1 address first, so if it is 2-5 we could overwrite last byte to make correct address
	nRF24L01_read_register(rfTransiever, RX_ADDR_P1, &(request.address), MAC_SIZE);
	switch (request.msg.pipe_number) {
		case 0: {
			nRF24L01_read_register(rfTransiever, RX_ADDR_P0, &(request.address), MAC_SIZE);
			break;
		}
		// pipe 1 address is already pre-filled, so do nothing
		case 1: break;
		// overwrite last byte of address
		case 2:
		case 3:
		case 4:
		case 5: {
			nRF24L01_read_register(rfTransiever, RX_ADDR_P2 - 2 + request.msg.pipe_number, &(request.address[MAC_SIZE-1]), 1);
			break;
		}
	}
	total_requests++;
	switch (RFMode) {
		default:
		case rmMaster: {
			// disable timeout, we've received response
			responseTimeout = -1;
			// no break
		}
		case rmIdle: {
			uartPacket.pkg.command = mcReceiveFromRF;
			uartPacket.pkg.payloadSize = MAC_SIZE + request.msg.length;
			memcpy(&(uartPacket.pkg.payload[0]), &(request.address[0]), MAC_SIZE);
			memcpy(&(uartPacket.pkg.payload[MAC_SIZE]), &(request.msg.data[0]), request.msg.length);
			uSendPacket(&uartPacket);
			break;
		}
		case rmSlave: {
			// in slave mode we need to respond to that packet and listen again
			parseRFPacket(&request);
			break;
		}
	}
}

void dataTransmitted() {
	// ack received, tx successful
	tRfPacket request;
	union uPackage uartPacket;
	//rfPacketsSent++;
	/*if (0x80 > lastSentPacketStatus) {
		ok_responses++;
	} else {
		error_responses++;
	}*/
	switch (RFMode) {
		default:
		case rmMaster: {
			// and here we're waiting for the response, but not forever
			responseTimeout = 20;
			RFListen(&ListenAddress);
			// no break
		}
		case rmIdle: {
			nRF24L01_read_register(rfTransiever, TX_ADDR, &(request.address), MAC_SIZE);
			/// TODO check if buffer has enough space for packet
			createUAckResponse(&(uartPacket), &(request.address[0]));
			uartPacket.pkg.payload[MAC_SIZE] = 0;
			uSendPacket(&uartPacket);
			//nRF24L01_listen(rfTransiever, 0, &(pipeAddress[0]));
			break;
		}
		case rmSlave: {
			// ack can be only for our response, so we know here transaction is done, we're back in listen state
			RFListen(&ListenAddress);
			break;
		}
	}
}

void transmissionFailed() {
	// no ack received n times
	tRfPacket request;
	union uPackage uartPacket;
	ack_timeouts++;
	switch (RFMode) {
		default:
		case rmIdle:
		case rmMaster: {
			nRF24L01_read_register(rfTransiever, TX_ADDR, &(request.address), MAC_SIZE);
			/// TODO check if buffer has enough space for packet
			createUAckResponse(&(uartPacket), &(request.address[0]));
			uartPacket.pkg.payload[MAC_SIZE] = 1;
			uSendPacket(&uartPacket);
			break;
		}
		case rmSlave: {
			// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
			RFListen(&ListenAddress);
			break;
		}
	}
}

void responseTimeoutEvent() {
	// no response from the requested slave
	union uPackage uartPacket;
	t_address addr;
	nRF24L01_read_register(rfTransiever, TX_ADDR, &(addr), MAC_SIZE);
	/// TODO check if buffer has enough space for packet
	createUAckResponse(&(uartPacket), &(addr[0]));
	uartPacket.pkg.payload[MAC_SIZE] = 2;
	uSendPacket(&uartPacket);
}

void msEvent() {
	if (0 <= responseTimeout) {
		if (0 > --responseTimeout) {
			responseTimeoutEvent();
		}
	}
}

void RFListen(t_address *address) {
	nRF24L01_listen(rfTransiever, 0, &((*address)[0]));
}

void transmitPacket(tRfPacket *packet) {
	nRF24L01_transmit(rfTransiever, packet->address, &(packet->msg));
}

eRFMode switchRFMode(eRFMode newMode) {
	if (rmGetMode <= newMode) {
		return RFMode;
	};
	RFMode = newMode;
	switch (RFMode) {
		case rmIdle: {
			break;
		}
		case rmSlave: {
			// start listen here
			RFListen(&ListenAddress);
			break;
		}
		case rmMaster: {
			break;
		}
		default:
			break;
	}
	return RFMode;
};

void setListenAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(ListenAddress, *address, MAC_SIZE);
	if (rmSlave == RFMode) {
		switchRFMode(RFMode);
	}
}

void parseRFPacket(tRfPacket *pkg) {
	tRfPacket response;
	// copy address
	memcpy(&response.address, pkg->address, sizeof(t_address));
	// minimum response size if 3 bytes: version, transaction id, response code
	// data is the last field, so its starting is minimal length of the packet
	generateResponse(pkg->msg.length, pkg->msg.data, &(response.msg.length), (uint8_t*) &(response.msg.data));
	//lastSentPacketStatus = response.msg.data[RF_RESP_CODE];
	transmitPacket(&response);
	// after that we're waiting for either ack from master or ack timeout
	// next action will be there
}
