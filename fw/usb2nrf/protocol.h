/*
 * protocol.h
 *
 * Created: 01.03.2020 22:24:15
 *  Author: Mintytail
 *
 * Main protocol file. Provides interface request-response for slave
 */ 


#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdbool.h>
#include <stdint.h>

#define PROTOCOL_VERSION 0


typedef struct {
	uint8_t length;
	uint8_t *data;
} sString;
// arguments are request and response
// caller should allocate space for the response
// unit (first argument) is guaranteed (by RF Parser) to be correct
typedef uint8_t (*fRFFunction)(uint8_t, sString*, sString*);

enum eResponseCodes {
	ercOk = 0,
	
	ercAddresBadLength = 1,
	
	ercBadUnit = 8,
	
	ercChBadChannels = 0x10,
	ercChResponseTooBig = 0x11,
	ercChBadPermissions = 0x12,
	ercChValidationFailed = 0x13,
	
	ercNotImplemented = 0x7F,
	ercBadVersion = 0x90,
	ercBadUnitId = 0xA0,
	ercNotConsecutiveTransactionId = 0xB0,
	ercBadFunctionId = 0xC0,
	ercResponseTooBig = 0xD0,

	ercBadRequestData = 0xE0,
};

typedef enum {
	eCDTBit = 0,
	eCDTUnsigned = 1,
	eCDTSigned = 2,
	eCDTArray = 3,
} eChannelDataType;

// 3 bytes
typedef struct s_channel {
	eChannelDataType dataType;
	union {
		bool *tBit;
		uint32_t *tUInt;
		int32_t *tInt;
		//string *tStr;
	} value;
} sChannel;

typedef enum {
	eUTSCustom = 0x00,
	eUTSSwitch = 0x01,
	eUTSAnalog = 0x02,
	eUTSLast,
	
	eUTACustom = 0x40,
	eUTARelay  = 0x41,
	eUTAAnalog = 0x42,
	eUTARGB    = 0x43,
	eUTALast
} eUnitType;

// 10 bytes long
typedef struct {
	eUnitType type;
	uint8_t channelsROLength;
	const sChannel *channelsRO;
	uint8_t channelsWOLength;
	const sChannel *channelsWO;
	uint8_t channelsRWLength;
	const sChannel *channelsRW;
} sUnit;

void generateResponse(uint8_t requestLength, uint8_t *requestData, uint8_t *responseLength, uint8_t *responseData);
void protocolInit();

#endif /* PROTOCOL_H_ */