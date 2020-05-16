/*
 * RF_model_slave.c
 *
 * Created: 16.05.2020 19:46:45
 *  Author: Mintytail
 */ 
#if BT_SLAVE == BUILD_TYPE
#include "../usb2nrf/RF model.h"
#ifndef UNIT_TESTING
	#include <avr/interrupt.h>
#else
	#include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>
#include <string.h>
#include "sstring.h"
#include "../usb2nrf/RF info.h"
#include "../usb2nrf/RF protocol.h"

static t_address ListenAddress;

void rf_slave_init() {
	protocolInit();
	init_rf_info();
	// TODO if we init in slave mode, we need to send "turn on" packet to master and make sure master received it
}

void dataReceived(sString *address, sString *payload) {
	// event, one per received packet
	total_requests++;
	// in slave mode we need to respond to that packet and listen again
	tRfPacket response;
	// copy address
	memcpy(&response.address, address->data, sizeof(t_address));
	// minimum response size if 3 bytes: version, transaction id, response code
	// data is the last field, so its starting is minimal length of the packet
	generateResponse(payload->length, payload->data, &(response.payloadLength), (uint8_t*) &(response.payloadData));
	nRF_transmit((uint8_t*)&(response.address), response.payloadLength, &(response.payloadData[0]));
	// after that we're waiting for either ack from master or ack timeout
}

void dataTransmitted(sString *address, sString *payload) {
	(void) payload; // *payload will contain ACK payload when we will support it
	// ack received, tx successful
	// ack can be only for our response, so we know here transaction is done, we're back in listen state
	nRF_setRFChannel(RFChannel);
	RFListen((uint8_t*) &ListenAddress);
}

void transmissionFailed(sString *address, sString *payload) {
	(void) payload;
	// no ack received n times
	ack_timeouts++;
	// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
	nRF_setRFChannel(RFChannel);
	RFListen((uint8_t*) &ListenAddress);
	ack_timeouts++;
}


void setListenAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(ListenAddress, *address, MAC_SIZE);
	nRF_setRFChannel(RFChannel);
	RFListen((uint8_t*) &ListenAddress);
}

#endif
