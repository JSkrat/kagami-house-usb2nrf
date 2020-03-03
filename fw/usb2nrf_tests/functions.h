/*
 * functions.h
 *
 * Created: 01.01.2020 14:54:46
 *  Author: Mintytail
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "messages.h"
#include <stdbool.h>

#define UNITS_LENGTH 6

typedef enum {
	eFSetSessionKey = 0,
	eFGetListOfUnits = 1,
	eFSetAddress = 2,
	eFGetRFStatistics = 3,
	
	eFGetPropertiesOfUnit = 0x10,
	eFGetTextDescription = 0x11,
	eFSetTextDescription = 0x12,
	
        eFReadUnitChannels = 0x20,
        eFWriteUnitChannels = 0x21,
        eFReadSingleUnitChannel = 0x22,
	// last item to get length of the array
	_eFCount
} eFunctions;

// arguments are request and response
// caller should allocate space for the response
// unit (first argument) is guaranteed (by RF Parser) to be correct
typedef uint8_t (*fRFFunction)(uint8_t, string*, string*);

extern const fRFFunction RFFunctions[_eFCount];

typedef enum {
	eCDTBit = 0,
	eCDTUnsigned = 1,
	eCDTSigned = 2,
	eCDTArray = 3,
} eChannelDataType;

typedef enum {
	eCReadable = 1,
	eCWritable = 2,
} eChannelRW;

// 3 bytes
typedef struct s_channel {
	eChannelDataType dataType;
	union {
		bool *tBit;
		uint32_t *tUInt;
		int32_t *tInt;
		string *tStr;
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

extern const sUnit PROGMEM units[];

// for unit testing only
uint8_t _readSingleUnitChannel(uint8_t unit, uint8_t request, uint8_t *response, uint8_t *responseLength);

#endif /* FUNCTIONS_H_ */