/*
 * RF_parser.c
 *
 * Created: 03.11.2019 19:17:51
 *  Author: Mintytail
 */
#include "RF parser.h"
#include "avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "avr-nrf24l01-master/src/nrf24l01.h"
#include "UART parser.h"
#include <avr/interrupt.h>
#include <string.h>

void checkTransieverRXBuf(/*const bool listenAfterwards*/);

nRF24L01 *rfTransiever;
eRFMode RFMode;
t_address MasterAddress;

ISR(PCINT0_vect) {
	// pin change, but we need only falling  edge
	if (portTransiever & (1 << poTransiever_IRQ)) return;
	checkTransieverRXBuf(/*true*/);
}

void rf_init() {
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
	RFMode = rmIdle;
	//nListen();
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

/**
 * check status of transmitted packet and queue according uart packet
 * ACK with code 0 (ack timeout), code 1 (ack received), no ack payload transmitted yet
 * and received data packets
 */
void checkTransieverRXBuf(/*const bool listenAfterwards*/) {
	nRF24L01Message msg;
	union uPackage uartPacket;
	uint8_t pipeAddress[MAC_SIZE];
	int txState = nRF24L01_transmit_success(rfTransiever);
	if (-1 == txState) {
		// no ack received n times
		//rfTimeouts++;
		nRF24L01_read_register(rfTransiever, TX_ADDR, &pipeAddress, MAC_SIZE);
		/// TODO check if buffer has enough space for packet
		createUAckResponse(&(uartPacket), &(pipeAddress[0]));
		uartPacket.pkg.payload[MAC_SIZE] = 1;
		uSendPacket(&uartPacket);
	} else if (0 == txState) {
		// ack received, tx successful
		//rfPacketsSent++;
		nRF24L01_read_register(rfTransiever, TX_ADDR, &pipeAddress, MAC_SIZE);
		/// TODO check if buffer has enough space for packet
		createUAckResponse(&(uartPacket), &(pipeAddress[0]));
		uartPacket.pkg.payload[MAC_SIZE] = 0;
		uSendPacket(&uartPacket);
		//nRF24L01_listen(rfTransiever, 0, &(pipeAddress[0]));
	}
	while (nRF24L01_data_received(rfTransiever)) {
		nRF24L01_read_received_data(rfTransiever, &msg);
		// assemble packet for uart
		/// TODO check if buffer has enough space for packet
		// read pipe 1 address first, so if it is 2-5 we could overwrite last byte to make correct address
		nRF24L01_read_register(rfTransiever, RX_ADDR_P1, &pipeAddress, MAC_SIZE);
		switch (msg.pipe_number) {
			case 0: {
				nRF24L01_read_register(rfTransiever, RX_ADDR_P0, &pipeAddress, MAC_SIZE);
				break;
			}
			case 1: break;
			// overwrite last byte of address
			case 2:
			case 3:
			case 4:
			case 5: {
				nRF24L01_read_register(rfTransiever, RX_ADDR_P2 - 2 + msg.pipe_number, &(pipeAddress[5]), 1);
				break;
			}
		}
		uartPacket.pkg.command = mcReceiveFromRF;
		uartPacket.pkg.payloadSize = MAC_SIZE + msg.length;
		memcpy(&(uartPacket.pkg.payload[0]), &(pipeAddress[0]), MAC_SIZE);
		memcpy(&(uartPacket.pkg.payload[MAC_SIZE]), &(msg.data[0]), msg.length);
		uSendPacket(&uartPacket);
	}
	//if (listenAfterwards)  nRF24L01_listen(rfTransiever, 0, &(replyAddress));
	//nListen();
}

void nListen(t_address *address) {
	nRF24L01_listen(rfTransiever, 0, &((*address)[0]));
}

void transmitPacket(tRfPacket *packet) {
	nRF24L01_transmit(rfTransiever, packet->address, &(packet->msg));
}

bool switchRFMode(eRFMode newMode) {
	if (rmBad <= newMode) {
		return false;
	};
	RFMode = newMode;
	switch (RFMode) {
		case rmIdle: {
			break;
		}
		case rmSlave: {
			// start listen here
			break;
		}
		case rmMaster: {
			break;
		}
		default:
			break;
	}
	return true;
};

void setMasterAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(MasterAddress, *address, MAC_SIZE);
	if (rmSlave == RFMode) {
		switchRFMode(RFMode);
	}
}
