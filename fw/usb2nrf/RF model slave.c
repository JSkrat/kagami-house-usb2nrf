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
	#include "Settings.h"
#else
	#include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>
#include <string.h>
#include "sstring.h"
#include "../usb2nrf/RF info.h"
#include "../usb2nrf/RF protocol.h"

static int16_t responseTimeout; // negative value means it is disabled, event triggered when it becomes disabled
static t_address ListenAddress;
static eModeType eMode;

void switchMode(eModeType newMode);
static void responseTimeoutEvent();

#ifndef UNIT_TESTING
ISR(TIMER0_COMPA_vect) {
	// this is being called every millisecond
	if (0 <= responseTimeout) {
		if (0 > --responseTimeout) {
			responseTimeoutEvent();
		}
	}
}
#endif

void rf_slave_init() {
	responseTimeout = -1;
#ifndef UNIT_TESTING
	// now let's setup timer
	TCCR0A = (1 << WGM01) | (0 << WGM00); // CTC mode
	TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << WGM02); // clk_io/64
	OCR0A = 250; // compare match interrupt every millisecond
	TIMSK0 = (1 << OCIE0A);
#endif
	protocolInit();
	init_rf_info();
	readSetting(esAddress, &ListenAddress);
	readSetting(esMode, &eMode);
	
	switchMode(eMode);
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
	switch (eMode) {
		case emNormalSlave: {
			nRF_setRFChannel(RFChannel);
			RFListen((uint8_t*) &ListenAddress);
			break;
		} 
		default:
		case emSearchMaster: {
			// do not switch rf channel in that mode
			responseTimeout = SLAVE_REQUEST_TIMEOUT_MS;
			RFListen((uint8_t*) &ListenAddress);
			break;
		}
	}
}

void transmissionFailed(sString *address, sString *payload) {
	// abandoned function, as we do not use auto acknowledge anymore
	// does not have proper reaction in "search master" mode
	(void) payload;
	// no ack received n times
	ack_timeouts++;
	// transiever should be set up in such a way, that if it timeouted, it is ok, we just give up.
	nRF_setRFChannel(RFChannel);
	RFListen((uint8_t*) &ListenAddress);
	ack_timeouts++;
}

static void responseTimeoutEvent() {
	// no response from the requested master, repeat request
	tRfPacket adv;
	generateAdvertisement(&(adv.payloadLength), (uint8_t*) &(adv.payloadData[0]));
	nRF_transmit((uint8_t*)&(ListenAddress), adv.payloadLength, &(adv.payloadData[0]));
}

void setListenAddress(t_address *address) {
	// write address and re-listen if we're slave
	memcpy(ListenAddress, *address, MAC_SIZE);
	nRF_setRFChannel(RFChannel);
	RFListen((uint8_t*) &ListenAddress);
}

void switchMode(eModeType newMode) {
	if (emNone == newMode || emAmount <= newMode) eMode = emSearchMaster;
	else eMode = newMode;
	switch (eMode) {
		case emNormalSlave: {
			responseTimeout = -1;
			nRF_setRFChannel(RFChannel);
			RFListen((uint8_t*) &ListenAddress);
			break;
		}
		default:
		case emSearchMaster: {
			responseTimeoutEvent();
			break;
		}
	}
}

#endif