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
#ifdef UNIT_TESTING
	#pragma pack(push, 1)
#endif
#include "sstring.h"

#define PROTOCOL_VERSION 0


// arguments are request and response
// caller should allocate space for the response
// unit (first argument) is guaranteed (by RF Parser) to be correct
typedef uint8_t (*fRFFunction)(const uint8_t, const scString*, sString*);

typedef struct {
	uint8_t rqVersion;
	uint8_t rqTransactionId;
	uint8_t rqUnitId;
	uint8_t rqNextChannel;
	uint8_t rqFunctionId;
	uint8_t rqData[];
} sRequest;

#define REQUEST_HEADER_SIZE ((uint8_t) offsetof(sRequest, rqData))
	
typedef struct {
	uint8_t rsVersion;
	uint8_t rsTransactionId;
	uint8_t rsCode;
	uint8_t rsData[];
} sResponse;

enum eResponseCodes {
	ercOk = 0,
	
	ercAddresBadLength = 1,
	
	ercChBadChannels = 0x10,
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

    eUTSystem  = 0x80,
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

void generateResponse(const uint8_t requestLength, const uint8_t *requestData, uint8_t *responseLength, uint8_t *responseData);
void protocolInit();

#ifdef UNIT_TESTING
	#pragma pack(pop)
#endif
#endif /* PROTOCOL_H_ */
