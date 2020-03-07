/*
 * functions.h
 *
 * Created: 01.01.2020 14:54:46
 *  Author: Mintytail
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../usb2nrf/protocol.h"
#include <stdint.h>

typedef enum {
	eFSetSessionKey = 0,
	eFGetListOfUnits = 1,
	eFSetAddress = 2,
	eFGetStatistics = 3,
    eFResetTransactionId = 4,
	eFNOP = 5,
	
	eFGetPropertiesOfUnit = 0x10,
	eFGetTextDescription = 0x11,
	eFSetTextDescription = 0x12,
	
    eFReadUnitChannels = 0x20,
    eFWriteUnitChannels = 0x21,
    eFReadSingleUnitChannel = 0x22,
	// last item to get length of the array
	_eFCount
} eFunctions;


extern const fRFFunction RFFunctions[_eFCount];

#endif /* FUNCTIONS_H_ */
