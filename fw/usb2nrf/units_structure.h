/*
 * units_structure.h
 *
 * Created: 09.02.2020 16:35:36
 *  Author: Mintytail
 */ 


#ifndef UNITS_STRUCTURE_H_
#define UNITS_STRUCTURE_H_
#include <stdbool.h>
#include <stdint.h>
#include "RF protocol.h"

#ifndef UNIT_TESTING
	#include <avr/pgmspace.h>
#else
    #include "../usb2nrf_tests/pgmspace.h"
#endif

typedef uint8_t (*fRFChannelFunction)(const uint8_t unit, const uint8_t channel, uint8_t* const responseLength, uint8_t* const response);

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

bool switches[5];
bool relays[2];
uint16_t analog_output;

#define UNITS_LENGTH 6
extern const sUnit PROGMEM units[UNITS_LENGTH];
//#define UNITS_LENGTH sizeof(units)/sizeof(sUnit)

#endif /* UNITS_STRUCTURE_H_ */
