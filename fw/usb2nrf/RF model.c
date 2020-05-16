/*
 * RF_parser.c
 *
 * Created: 03.11.2019 19:17:51
 *  Author: Mintytail
 */
#include "../usb2nrf/RF model.h"
#include "../usb2nrf/nRF model.h"
#ifndef UNIT_TESTING
    #include <avr/interrupt.h>
#else
    #include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>
#include <string.h>
#include "sstring.h"

void dataTransmitted(sString *address, sString *payload);
void dataReceived(sString *address, sString *payload);
void transmissionFailed(sString *address, sString *payload);

void rf_master_init();
void rf_slave_init();

static eRFMode RFMode;
static t_address ListenAddress;
uint8_t RFChannel;

void rf_init() {
	// setup nrf interface
	nRF_init();
	cNRF_DataReceived = &dataReceived;
	cNRF_DataTransmitted = &dataTransmitted;
	cNRF_TransmissionFailed = &transmissionFailed;
	//RFMode = rmIdle;
	RFChannel = 2; // the default one for nrf24l01
#if BT_MASTER == BUILD_TYPE
	rf_master_init();
#elif BT_SLAVE == BUILD_TYPE
	rf_slave_init();
#endif
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
			nRF_setRFChannel(RFChannel);
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

void RFTransmit(tRfPacket *packet) {
	// save the address so we could listen that address after request is sent
	memcpy(ListenAddress, packet->address, MAC_SIZE);
	nRF_transmit((uint8_t*)&(packet->address), packet->payloadLength, (uint8_t*)&(packet->payloadData));
};

bool RFSetChannel(uint8_t channel) {
#if BT_MASTER == BUILD_TYPE || BT_DEBUG == BUILD_TYPE
	if (nRF_setRFChannel(channel)) {
#elif BT_SLAVE == BUILD_TYPE
	if (nRF_validate_rf_channel(channel)) {
#endif
		RFChannel = channel;
		return true;
	} else {
		return false;	
	}
}
