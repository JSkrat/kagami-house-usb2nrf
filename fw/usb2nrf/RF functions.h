/*
 * functions.h
 *
 * Created: 01.01.2020 14:54:46
 *  Author: Mintytail
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../usb2nrf/RF protocol.h"
#include <stdint.h>

typedef enum {
	eFSetSessionKey = 0,
	eFGetListOfUnits = 1,
	eFSetAddress = 2,
	eFGetStatistics = 3,
    eFResetTransactionId = 4,
	eFNOP = 5,
	eFSetRFChannel = 6,
	eFSetMode = 7,
	
	eFGetPropertiesOfUnit = 0x10,
	eFGetTextDescription = 0x11,
	eFSetTextDescription = 0x12,
	
    eFReadUnitChannels = 0x20,
    eFWriteUnitChannels = 0x21,
    eFReadSingleUnitChannel = 0x22,
} eFunctions;

typedef struct {
	eFunctions functionCode;
	fRFFunction function;
} tRFCodeFunctionItem;

//extern const fRFFunction RFFunctions[_eFCount];
#define _eFCount 14
extern const tRFCodeFunctionItem RFFunctions[_eFCount];

#endif /* FUNCTIONS_H_ */
