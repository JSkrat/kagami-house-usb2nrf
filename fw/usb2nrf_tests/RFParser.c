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
#include "../usb2nrf/functions.h"

void checkTransieverRXBuf(/*const bool listenAfterwards*/);
void parseRFPacket(tRfPacket *pkg);

nRF24L01 *rfTransiever;
eRFMode RFMode;
t_address ListenAddress;
uint8_t lastReceivedTransacrionId;
uint16_t total_requests, ok_responses, error_responses, transaction_errors, ack_timeouts;

enum eRFError {
	ereNone = 0,
	ereBadVersion = 0x90,
	ereBadUnitId = 0xA0,
	ereNotConsecutiveTransactionId = 0xB0,
	ereBadFunctionId = 0xC0,
	ereResponseTooBig = 0xD0,
	
} lastSentPacketStatus;

#ifndef UNIT_TESTING
ISR(PCINT0_vect) {
	// pin change, but we need only falling  edge
	if (portTransiever & (1 << poTransiever_IRQ)) return;
	checkTransieverRXBuf(/*true*/);
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
#endif
	RFMode = rmIdle;
	lastReceivedTransacrionId = 0;
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

/**
 * check status of transmitted packet and queue according uart packet
 * ACK with code 0 (ack timeout), code 1 (ack received), no ack payload transmitted yet
 * and received data packets
 */
void checkTransieverRXBuf(/*const bool listenAfterwards*/) {
	tRfPacket request;
	//nRF24L01Message msg;
	union uPackage uartPacket;
	//uint8_t pipeAddress[MAC_SIZE];
	// this will return -2 if there was no ack packet received/timeouted
	int txState = nRF24L01_transmit_success(rfTransiever);
	if (-1 == txState) {
		// no ack received n times
		ack_timeouts++;
		if (rmIdle == RFMode || rmMaster == RFMode) {
			nRF24L01_read_register(rfTransiever, TX_ADDR, &(request.address), MAC_SIZE);
			/// TODO check if buffer has enough space for packet
			createUAckResponse(&(uartPacket), &(request.address[0]));
			uartPacket.pkg.payload[MAC_SIZE] = 1;
			uSendPacket(&uartPacket);
		} else if (rmSlave == RFMode) {
			// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
			nListen(&ListenAddress);
		}
	} else if (0 == txState) {
		// ack received, tx successful
		//rfPacketsSent++;
		switch (lastSentPacketStatus) {
			case ereNotConsecutiveTransactionId: {
				transaction_errors++;
			}
			default: {
				if (0x80 > lastSentPacketStatus) {
					ok_responses++;
				} else {
					error_responses++;
				}
			}
		}
		if (rmIdle == RFMode || rmMaster == RFMode) {
			nRF24L01_read_register(rfTransiever, TX_ADDR, &(request.address), MAC_SIZE);
			/// TODO check if buffer has enough space for packet
			createUAckResponse(&(uartPacket), &(request.address[0]));
			uartPacket.pkg.payload[MAC_SIZE] = 0;
			uSendPacket(&uartPacket);
			//nRF24L01_listen(rfTransiever, 0, &(pipeAddress[0]));
		} else if (rmSlave == RFMode) {
			// ack can be only for our response, so we know here transaction is done, we're back in listen state
			nListen(&ListenAddress);
		}
	}
	while (nRF24L01_data_received(rfTransiever)) {
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
		if (rmIdle == RFMode || rmMaster == RFMode) {
			uartPacket.pkg.command = mcReceiveFromRF;
			uartPacket.pkg.payloadSize = MAC_SIZE + request.msg.length;
			memcpy(&(uartPacket.pkg.payload[0]), &(request.address[0]), MAC_SIZE);
			memcpy(&(uartPacket.pkg.payload[MAC_SIZE]), &(request.msg.data[0]), request.msg.length);
			uSendPacket(&uartPacket);
		} else {
			// in slave mode we need to respond to that packet and listen again
			parseRFPacket(&request);
		}
	}
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
			nListen(&ListenAddress);
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

void setListenAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(ListenAddress, *address, MAC_SIZE);
	if (rmSlave == RFMode) {
		switchRFMode(RFMode);
	}
}

enum eRFError validatePacket(tRfPacket *pkg) {
	//enum eRFError RFError = ereNone;
	if (0 != pkg->msg.data[RF_VERSION]) {
		return ereBadVersion;
	}
	if (lastReceivedTransacrionId+1 != pkg->msg.data[RF_TRANSACTION_ID]) {
		return ereNotConsecutiveTransactionId;
	}
	// only basic check if unit 0 functions called for not unit 0 and vice versa
	if ((0 == pkg->msg.data[RF_UNIT_ID]) != (0x10 > pkg->msg.data[RF_FUNCTION_ID])) {
		return ereBadFunctionId;
	}
	// check if unit exists
	if (UNITS_LENGTH <= pkg->msg.data[RF_UNIT_ID]) {
		return ereBadUnitId;
	}
	return ereNone;
}

void parseRFPacket(tRfPacket *pkg) {
	enum eRFError validation = validatePacket(pkg);
	tRfPacket response;
	// copy address, version, transaction id
	memcpy(&response, pkg, sizeof(tRfPacket)); // should be 39 bytes
	// minimum response size if 3 bytes: version, transaction id, response code
	// data is the last field, so its starting is minimal length of the packet
	response.msg.length = RF_RESP_DATA;
	response.msg.data[RF_RESP_CODE] = validation;
	switch (validation) {
		case ereNone: {
			// process packet here
			fRFFunction method = pgm_read_ptr_near(RFFunctions[pkg->msg.data[RF_FUNCTION_ID]]);
			if (NULL == method) {
				response.msg.data[RF_RESP_CODE] = ereBadFunctionId;
				break;
			}
			response.msg.data[RF_RESP_CODE] = (*method)(
				pkg->msg.data[RF_UNIT_ID],
				(string*) &(pkg->msg.data[RF_DATA]), 
				(string*) &(response.msg.data[RF_RESP_DATA])
			);
			// check here for the max length of data to fit into packet
			if (RF_RESP_DATA_LENGTH < response.msg.length) {
				response.msg.data[RF_RESP_CODE] = ereResponseTooBig;
				// data is the last field, so its starting is minimal length of the packet
				response.msg.length = RF_RESP_DATA;
			}
			break;
		}
		case ereNotConsecutiveTransactionId: {
			response.msg.length++;
			response.msg.data[RF_RESP_DATA] = lastReceivedTransacrionId;
			break;
		}
		default: {
			break;
		}
	}
	lastSentPacketStatus = response.msg.data[RF_RESP_CODE];
	transmitPacket(&response);
	// after that we're waiting for either ack from master or ack timeout
	// next action will be there
}
