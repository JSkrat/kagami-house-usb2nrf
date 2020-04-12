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
#ifdef UNIT_TESTING
	#pragma pack(1, push)
#endif

#include "../usb2nrf/defines.h"
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/nRF model.h"


typedef enum {
	rmIdle = 0,
	rmSlave = 1,
	rmMaster = 2,
	rmGetMode, // should be the last, we check for validity by mode being numerically less than that value
} eRFMode;

//extern const eRFMode RFMode;
//extern t_address MasterAddress;

typedef uint8_t t_address[MAC_SIZE];

typedef enum {
	eptData = 0,
	eptAckOk = 1,
	eptAckTimeout = 2,
	eptResponseTimeout = 3,
} ePacketType;

// 40 bytes long, no pointers
typedef struct {
	t_address address;
	ePacketType type;
	nRF24L01Message msg;
} tRfPacket;

void rf_init();
void checkTransieverRXBuf();
void transmitPacket(tRfPacket *packet);
void RFListen(t_address *address);
eRFMode switchRFMode(eRFMode newMode);
void setListenAddress(t_address *address);

#ifdef UNIT_TESTING
	#pragma pack(pop)
#endif
#endif /* RF_PARSER_H_ */