/*
 * RF_parser.h
 *
 * Created: 03.11.2019 19:18:07
 *  Author: Mintytail
 *
 * The main job of this module is to create interface request-response from both, master and slave points of view
 * from the nrf24l01 library
 */ 


#ifndef RF_PARSER_H_
#define RF_PARSER_H_

#include "../usb2nrf/defines.h"
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
	rmGetMode, // should be the last, we check for validity by mode being numerically less than that value
} eRFMode;

//extern const eRFMode RFMode;
//extern t_address MasterAddress;
extern uint16_t total_requests, ok_responses, error_responses, transaction_errors, ack_timeouts;

void rf_init();
void checkTransieverRXBuf();
void transmitPacket(tRfPacket *packet);
void RFListen(t_address *address);
eRFMode switchRFMode(eRFMode newMode);
void setListenAddress(t_address *address);



#endif /* RF_PARSER_H_ */
