/*
 * RF_parser.c
 *
 * Created: 03.11.2019 19:17:51
 *  Author: Mintytail
 */
#include "../usb2nrf/RF model.h"
#include "../usb2nrf/nRF model.h"
#include "../usb2nrf/RF info.h"
#ifndef UNIT_TESTING
    #include <avr/interrupt.h>
#else
    #include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>
#include <string.h>
#include "../usb2nrf/RF protocol.h"
#include "sstring.h"

void checkTransieverRXBuf(void);
void dataTransmitted(sString *address, sString *payload);
void dataReceived(sString *address, sString *payload);
void transmissionFailed(sString *address, sString *payload);
void responseTimeoutEvent(void);
void msEvent(void);

static eRFMode RFMode;
static t_address ListenAddress;
static int responseTimeout = -1; // negative value means it is disabled, event triggered when it becomes disabled

#define RFBUFFER_SIZE 8
static tRfPacket RFBuffer[RFBUFFER_SIZE];
// push to the end, pop from the begin
// begin points to the next item
// end points to the next free space
static int rfbBegin = 0, rfbEnd = 0, rfbSize = 0;

#ifndef UNIT_TESTING
ISR(TIMER0_COMPA_vect) {
	// this is being called every millisecond
	msEvent();
}
#endif

void rf_init() {
	// setup nrf interface
	nRF_init();
	cNRF_DataReceived = &dataReceived;
	cNRF_DataTransmitted = &dataTransmitted;
	cNRF_TransmissionFailed = &transmissionFailed;
#ifndef UNIT_TESTING
	// now let's setup timer
	TCCR0A = (1 << WGM01) | (0 << WGM00); // CTC mode
	TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << WGM02); // clk_io/64
	OCR0A = 250; // compare match interrupt every millisecond
	TIMSK0 = (1 << OCIE0A);
#endif
	RFMode = rmIdle;
	responseTimeout = -1;
	protocolInit();
	// reset buffer
	rfbBegin = 0; rfbEnd = 0; rfbSize = 0;
	init_rf_info();
	// TODO if we init in slave mode, we need to send "turn on" packet to master and make sure master received it
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
	if (0 >= rfbSize) return NULL;
	rfbSize--;
	tRfPacket *ret = &(RFBuffer[rfbBegin++]);
	if (RFBUFFER_SIZE <= rfbBegin) rfbBegin = 0;
	return ret;
}

void dataReceived(sString *address, sString *payload) {
	// event, one per received packet
	tRfPacket *request = nextRFBufferFreeElement();
    if (NULL == request) {
        missed_packets++;
        return;
    }
	request->type = eptData;
	memcpy(request->address, address->data, address->length);
	request->payloadLength = payload->length;
	memcpy(&(request->payloadData), payload->data, payload->length);
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
			total_requests++;
			// in slave mode we need to respond to that packet and listen again
			tRfPacket response;
			// copy address
			memcpy(&response.address, request->address, sizeof(t_address));
			// minimum response size if 3 bytes: version, transaction id, response code
			// data is the last field, so its starting is minimal length of the packet
			generateResponse(request->payloadLength, request->payloadData, &(response.payloadLength), (uint8_t*) &(response.payloadData));
			nRF_transmit((uint8_t*)&(response.address), response.payloadLength, &(response.payloadData[0]));
			// after that we're waiting for either ack from master or ack timeout

			// also remove pushed to the rf buffer packet, no one is gonna read it
			nextRFBufferElement();
			break;
		}
	}
}

void dataTransmitted(sString *address, sString *payload) {
	(void) payload; // *payload will contain ACK payload when we will support it
	// ack received, tx successful
	tRfPacket *request = nextRFBufferFreeElement();
    if (NULL == request) {
        missed_packets++;
        return;
    }
	request->type = eptAckOk;
	request->payloadLength = 0;
    memcpy(&(request->address), address->data, MAC_SIZE);
	switch (RFMode) {
		default:
		case rmMaster: {
			// and here we're waiting for the response, but not forever
			responseTimeout = SLAVE_RESPONSE_TIMEOUT_MS;
			RFListen((uint8_t*) &ListenAddress);
			// no break
		}
		case rmIdle: {
			break;
		}
		case rmSlave: {
			// ack can be only for our response, so we know here transaction is done, we're back in listen state
			RFListen((uint8_t*) &ListenAddress);
			// pop out pushed to rf buffer packet, no one is gonna read it
			nextRFBufferElement();
			break;
		}
	}
}

void transmissionFailed(sString *address, sString *payload) {
	(void) payload;
	// no ack received n times
	tRfPacket *request = nextRFBufferFreeElement();
    if (NULL == request) {
        missed_packets++;
        return;
    }
	request->type = eptAckTimeout;
	request->payloadLength = 0;
	ack_timeouts++;
	switch (RFMode) {
		default:
		case rmIdle:
		case rmMaster: {
            memcpy(&(request->address), address->data, MAC_SIZE);
			break;
		}
		case rmSlave: {
			// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
			RFListen((uint8_t*) &ListenAddress);
			nextRFBufferElement();
			ack_timeouts++;
			break;
		}
	}
}

void responseTimeoutEvent() {
	// no response from the requested slave
	tRfPacket *packet = nextRFBufferFreeElement();
    if (NULL == packet) {
        missed_packets++;
        return;
    }
	packet->type = eptResponseTimeout;
	packet->payloadLength = 0;
	memcpy(&(packet->address), ListenAddress, MAC_SIZE);
}

void msEvent() {
	if (0 <= responseTimeout) {
		if (0 > --responseTimeout) {
			responseTimeoutEvent();
		}
	}
}

eRFMode switchRFMode(eRFMode newMode) {
	if (rmGetMode <= newMode) {
		return RFMode;
    }
	RFMode = newMode;
	switch (RFMode) {
		case rmIdle: {
			nRF_go_idle();
			break;
		}
		case rmSlave: {
			// start listen here
			RFListen((uint8_t*)&ListenAddress);
			break;
		}
		case rmMaster: {
			nRF_go_idle();
			break;
		}
		default:
			break;
	}
	return RFMode;
}

void setListenAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(ListenAddress, *address, MAC_SIZE);
	if (rmSlave == RFMode) {
		switchRFMode(RFMode);
	}
}

void RFTransmit(tRfPacket *packet) {
	// save the address so we could listen that address after request is sent
	memcpy(ListenAddress, packet->address, MAC_SIZE);
	nRF_transmit((uint8_t*)&(packet->address), packet->payloadLength, (uint8_t*)&(packet->payloadData));
};
