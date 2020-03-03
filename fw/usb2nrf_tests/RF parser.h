/*
 * RF_parser.h
 *
 * Created: 03.11.2019 19:18:07
 *  Author: Mintytail
 */ 


#ifndef RF_PARSER_H_
#define RF_PARSER_H_

#include "defines.h"
#include "../usb2nrf/nrf24l01.h"

extern nRF24L01 *rfTransiever;

typedef uint8_t t_address[MAC_SIZE];

typedef struct {
	t_address address;
	nRF24L01Message msg;
} tRfPacket;

typedef enum {
	rmIdle = 0,
	rmSlave = 1,
	rmMaster = 2,
	rmBad,
} eRFMode;

extern eRFMode RFMode;
//extern t_address MasterAddress;
extern uint16_t total_requests, ok_responses, error_responses, transaction_errors, ack_timeouts;

void rf_init();
void checkTransieverRXBuf();
void transmitPacket(tRfPacket *packet);
void nListen(t_address *address);
bool switchRFMode(eRFMode newMode);
void setListenAddress(t_address *address);



#endif /* RF_PARSER_H_ */