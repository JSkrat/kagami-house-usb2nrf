/*
 * RF_parser.c
 *
 * Created: 03.11.2019 19:17:51
 *  Author: Mintytail
 */
#include "../usb2nrf/RF model.h"
#include "../usb2nrf/nRF model.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"
#ifndef UNIT_TESTING
    #include <avr/interrupt.h>
#endif
#include <stdint.h>
#include <string.h>
#include "../usb2nrf/RF protocol.h"

void checkTransieverRXBuf(/*const bool listenAfterwards*/);
void parseRFPacket(tRfPacket *pkg);
void dataTransmitted();
void dataReceived();
void transmissionFailed();
void responseTimeoutEvent();
void msEvent();

eRFMode RFMode;
t_address ListenAddress;
int responseTimeout = -1; // negative value means it is disabled, event triggered when it becomes disabled

#define RFBUFFER_SIZE 8
tRfPacket RFBuffer[RFBUFFER_SIZE];
// push to the end, pop from the begin
// begin points to the next item
// end points to the next free space
int rfbBegin = 0, rfbEnd = 0, rfbSize = 0;

#ifndef UNIT_TESTING
ISR(TIMER0_COMPA_vect) {
	// this is being called every millisecond
	msEvent();
}
#endif

void rf_init() {
#ifndef UNIT_TESTING
	// setup nrf interface
	nRF_init();
	cNRF_DataReceived = &dataReceived;
	cNRF_DataTransmitted = &dataTransmitted;
	cNRF_TransmissionFailed = &transmissionFailed;
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
 * RF buffer functions
 */
tRfPacket* nextRFBufferFreeElement() {
	if (RFBUFFER_SIZE <= rfbSize++) {
		rfbSize--;
		return NULL;
	}
	tRfPacket *ret = &(RFBuffer[rfbEnd++]);
	if (RFBUFFER_SIZE <= rfbEnd) rfbEnd = 0;
	return ret;
}

tRfPacket* nextRFBufferElement() {
	if (! rfbSize) return NULL;
	rfbSize--;
	tRfPacket *ret = &(RFBuffer[rfbBegin++]);
	if (RFBUFFER_SIZE <= rfbBegin) rfbBegin = 0;
	return ret;
}

void dataReceived(void) {
	// event, one per received packet
	tRfPacket *request = nextRFBufferFreeElement();
	/// @TODO check if request is NULL
	request->type = eptData;
	nRF24L01_read_received_data(rfTransiever, &(request->msg));
	// read pipe 1 address first, so if it is 2-5 we could overwrite last byte to make correct address
	nRF24L01_read_register(rfTransiever, RX_ADDR_P1, &(request->address), MAC_SIZE);
	switch (request->msg.pipe_number) {
		case 0: {
			nRF24L01_read_register(rfTransiever, RX_ADDR_P0, &(request->address), MAC_SIZE);
			break;
		}
		// pipe 1 address is already pre-filled, so do nothing
		case 1: break;
		// overwrite last byte of address
		case 2:
		case 3:
		case 4:
		case 5: {
			nRF24L01_read_register(rfTransiever, RX_ADDR_P2 - 2 + request->msg.pipe_number, &(request->address[MAC_SIZE-1]), 1);
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
			break;
		}
		case rmSlave: {
			// in slave mode we need to respond to that packet and listen again
			parseRFPacket(request);
			// also remove pushed to the rf buffer packet, no one is gonna read it
			nextRFBufferElement();
			break;
		}
	}
}

void dataTransmitted(void) {
	// ack received, tx successful
	tRfPacket *request = nextRFBufferFreeElement();
	/// @TODO check result for NULL
	//rfPacketsSent++;
	/*if (0x80 > lastSentPacketStatus) {
		ok_responses++;
	} else {
		error_responses++;
	}*/
	request->type = eptAckOk;
	nRF24L01_read_register(rfTransiever, TX_ADDR, &(request->address), MAC_SIZE);
	switch (RFMode) {
		default:
		case rmMaster: {
			// and here we're waiting for the response, but not forever
			responseTimeout = 20;
			RFListen(&ListenAddress);
			// no break
		}
		case rmIdle: {
			break;
		}
		case rmSlave: {
			// ack can be only for our response, so we know here transaction is done, we're back in listen state
			RFListen(&ListenAddress);
			// pop out pushed to rf buffer packet, no one is gonna read it
			nextRFBufferElement();
			break;
		}
	}
}

void transmissionFailed(void) {
	// no ack received n times
	tRfPacket *request = nextRFBufferFreeElement();
	request->type = eptAckTimeout;
	ack_timeouts++;
	switch (RFMode) {
		default:
		case rmIdle:
		case rmMaster: {
			nRF24L01_read_register(rfTransiever, TX_ADDR, &(request->address), MAC_SIZE);
			break;
		}
		case rmSlave: {
			// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
			RFListen(&ListenAddress);
			nextRFBufferElement();
			break;
		}
	}
}

void responseTimeoutEvent() {
	// no response from the requested slave
	tRfPacket *packet = nextRFBufferFreeElement();
	packet->type = eptResponseTimeout;
	nRF24L01_read_register(rfTransiever, TX_ADDR, &(packet->address), MAC_SIZE);
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

/**
 * process received packet as a slave
 */
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
