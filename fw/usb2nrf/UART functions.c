/*
 * UART_functions.c
 *
 * Created: 13.04.2020 22:07:48
 *  Author: Mintytail
 */ 
#include "UART functions.h"
#include "string.h"
#include "UART protocol.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"
#include "../usb2nrf/nRF model.h"
#include "../usb2nrf/RF model.h"
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

uint8_t uStatus(const scString *request, sString *response) {
	response->length = 0x13;
	for (int i = 0x00; i <= 0x09; i++) {
		nRF24L01_read_register(rfTransiever, i, &(response->data[i]), 1);
	}
	for (int i = 0x11; i <= 0x17; i++) {
		nRF24L01_read_register(rfTransiever, i, &(response->data[i - 0x11 + 0x0A]), 1);
	}
	for (int i = 0x1C; i <= 0x1D; i++) {
		nRF24L01_read_register(rfTransiever, i, &(response->data[i - 0x1C + 0x11]), 1);
	}
	//*((uint16_t*) (respBuffer.pkg.payload+1)) = rfPacketsSent;
	//*((uint16_t*) (respBuffer.pkg.payload+3)) = rfTimeouts;
	//*((uint16_t*) (respBuffer.pkg.payload+5)) = badRFPackets;
	return eucOk;
}

uint8_t uAddresses(const scString *request, sString *response) {
	response->length = 0x13;
	nRF24L01_read_register(rfTransiever, 0x0A, &(response->data[0x00]), 5);
	nRF24L01_read_register(rfTransiever, 0x0B, &(response->data[0x05]), 5);
	for (int i = 0x0C; i < 0x10; i++) {
		nRF24L01_read_register(rfTransiever, i, &(response->data[i - 0x0C + 0x0A]), 1);
	}
	nRF24L01_read_register(rfTransiever, 0x10, &(response->data[0x0E]), 5);
	return eucOk;
}

uint8_t uSetChannel(const scString *request, sString *response) {
	nRF24L01_write_register(rfTransiever, RF_CH, (uint8_t*) &(request->data[0]), 1);
	return eucOk;
}

uint8_t uSetBitRate(const scString *request, sString *response) {
	uint8_t rf_setup; 
	nRF24L01_read_register(rfTransiever, RF_SETUP, &rf_setup, 1);
	rf_setup &= ~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH));
	if (request->data[0] & 0b00000010) rf_setup |= (1 << RF_DR_LOW);
	if (request->data[0] & 0b00000001) rf_setup |= (1 << RF_DR_HIGH);
	nRF24L01_write_register(rfTransiever, RF_SETUP, &rf_setup, 1);
	return eucOk;
}

uint8_t uSetTxPower(const scString *request, sString *response) {
	uint8_t rf_setup;
	nRF24L01_read_register(rfTransiever, RF_SETUP, &rf_setup, 1);
	rf_setup &= ~(0b00000011 << RF_PWR);
	rf_setup |= (request->data[0] & 0b00000011) << RF_PWR;
	nRF24L01_write_register(rfTransiever, RF_SETUP, &rf_setup, 1);
	return eucOk;
}

uint8_t uClearTx(const scString *request, sString *response) {
	nRF24L01_flush_transmit_message(rfTransiever);
	return eucOk;
}

uint8_t uListen(const scString *request, sString *response) {
	RFListen((t_address *) request->data);
	return eucOk;
}

uint8_t uSetMode(const scString *request, sString *response) {
	response->length = 1;
	eRFMode setMode = request->data[0];
	eRFMode newMode = switchRFMode(setMode);
	response->data[0] = newMode;
	if (setMode == newMode) {
		return eucOk;
	} else {
		return eucGeneralFail;
	}
}

uint8_t uSetListenAddress(const scString *request, sString *response) {
	setListenAddress((t_address *) request->data);
	return eucOk;
}

uint8_t uTransmit(const scString *request, sString *response) {
	response->length = 1;
	if (MAC_SIZE > request->length) {
		return eucArgumentValidationError;
	}
	//checkTransieverRXBuf();
	_delay_us(10);
	tRfPacket packet;
	memcpy(packet.address, request->data, MAC_SIZE);
	packet.msg.length = request->length - MAC_SIZE;
	if (packet.msg.length) {
		memcpy(packet.msg.data, request->data + MAC_SIZE, packet.msg.length);
	}
	transmitPacket(&packet);
	return eucOk;
}

const PROGMEM tUARTCommandItem UARTFunctions[UART_FUNCTIONS_NUMBER] = {
	{ mcStatus, &uStatus },
	{ mcAddresses, &uAddresses },
		
	{ mcSetChannel, &uSetChannel },
	{ mcSetTXPower, &uSetTxPower },
	{ mcSetBitRate, &uSetBitRate },
	{ mcSetAutoRetransmitDelay, NULL },
	{ mcSetAutoRetransmitCount, NULL },
		
	{ mcClearTX, &uClearTx },
	{ mcClearRX, NULL },
		
	{ mcListen, &uListen },
		
	{ mcSetMode, &uSetMode },
	{ mcSetListenAddress, &uSetListenAddress },
	
	{ mcReadRFBuffer, NULL },
	
	{ mcTransmit, &uTransmit },
};
