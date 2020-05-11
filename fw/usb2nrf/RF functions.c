/*
 * functions.c
 *
 * Created: 01.01.2020 14:54:34
 *  Author: Mintytail
 */ 

#include "../usb2nrf/RF functions.h"
#include "../usb2nrf/RF protocol.h"
#include "../usb2nrf/RF info.h"
#ifndef UNIT_TESTING
    #include "./units_structure.h"
    #include "./RF model.h"
    #include <avr/pgmspace.h>
#else
    #include "../usb2nrf_tests/units_structure.h"
    #include "../usb2nrf/RF model.h"
    #include "../usb2nrf_tests/pgmspace.h"
#endif
#include "../usb2nrf/RF protocol internal.h"
#include <stdbool.h>
#include <stdint.h>
// for memcpy
#include <string.h>
#include "sstring.h"
// if it is unit tests, it should be local file, not of main project one

const uint8_t _dataTypeLength[] = {
	[eCDTBit] = 1,
	[eCDTUnsigned] = 4,
	[eCDTSigned] = 4,
	[eCDTArray] = 1,
};

uint8_t setSessionKey(const uint8_t unit, const scString *request, sString *response) {
	// TODO after integrating cipher library
    (void) unit;
    (void) request;
    (void) response;
	return ercNotImplemented;
}

uint8_t getListOfUnits(const uint8_t unit, const scString *request, sString *response) {
    (void) unit;
    (void) request;
	response->length = 0 + 4*UNITS_LENGTH;
	//response->data[0] = UNITS_LENGTH;
	for (int i = 0; i < UNITS_LENGTH; i++) {
		uint8_t base = 0 + 4*i;
        response->data[base+0] = pgm_read_byte((const uint8_t*) &(units[i].type));
        response->data[base+1] = pgm_read_byte((const uint8_t*) &(units[i].channelsROLength));
        response->data[base+2] = pgm_read_byte((const uint8_t*) &(units[i].channelsWOLength));
        response->data[base+3] = pgm_read_byte((const uint8_t*) &(units[i].channelsRWLength));
	}
	return ercOk;
}

uint8_t setMACAddress(const uint8_t unit, const scString *request, sString *response) {
    (void) unit;
    (void) response;
//    response->length = 0;
	if (5 == request->length) {
		setListenAddress((t_address*) request->data);	
		return ercOk;
	} else {
		return ercAddresBadLength;
	}
}

uint8_t resetTransactionId(const uint8_t unit, const scString *request, sString *response) {
	(void) unit;
	(void) request;
	lastTransacrionId = 128;
	response->data[0] = lastTransacrionId;
	response->length = 1;
	return ercOk;
}

uint8_t rfNOP(const uint8_t unit, const scString *request, sString *response) {
	(void) unit;
	(void) request;
	(void) response;
	return ercOk;
}

uint8_t getStatistics(const uint8_t unit, const scString *request, sString *response) {
    (void) unit;
    (void) request;
	response->length = 12;
    #define STORE_16(index, value) response->data[index] = value & 0xFF; response->data[index+1] = ((uint16_t)value) >> 8
	STORE_16(0, total_requests);
	STORE_16(2, ok_responses);
	STORE_16(4, error_responses);
    STORE_16(6, missed_packets);
	STORE_16(8, ack_timeouts);
	STORE_16(10, validation_errors);
	return ercOk;
}

uint8_t getPropertiesOfUnit(const uint8_t unit, const scString *request, sString *response) {
    (void) request;
	response->length = 13;
    response->data[0] = pgm_read_byte((const uint8_t*) &(units[unit].type));
    const uint8_t ro_num = pgm_read_byte(&(units[unit].channelsROLength));
    const uint8_t wo_num = pgm_read_byte(&(units[unit].channelsWOLength));
    const uint8_t rw_num = pgm_read_byte(&(units[unit].channelsRWLength));
    const sChannel *ro_ch = pgm_read_ptr(&(units[unit].channelsRO));
    const sChannel *wo_ch = pgm_read_ptr(&(units[unit].channelsWO));
    const sChannel *rw_ch = pgm_read_ptr(&(units[unit].channelsRW));
	for (int i = 0; i < 4; i++) {
		uint8_t ro = 0;
		uint8_t wo = 0;
		uint8_t rw = 0;
		for (int j = 0; j < 4; j++) {
			uint8_t index = i*4 + j;
			ro <<= 2;
			wo <<= 2;
			rw <<= 2;
            if (ro_num > index)
                ro |= pgm_read_byte((const uint8_t*) &(ro_ch[index].dataType));
            if (wo_num > index)
                wo |= pgm_read_byte((const uint8_t*) &(wo_ch[index].dataType));
            if (rw_num > index)
                rw |= pgm_read_byte((const uint8_t*) &(rw_ch[index].dataType));
		}
		response->data[1 + i + 4*0] = ro;
		response->data[1 + i + 4*1] = wo;
		response->data[1 + i + 4*2] = rw;
	}
	return ercOk;
}

uint8_t getTextDescription(const uint8_t unit, const scString *request, sString *response) {
	return ercNotImplemented;
}

uint8_t setTextDescription(const uint8_t unit, const scString *request, sString *response) {
	return ercNotImplemented;
}

uint8_t _readWriteSingleUnitChannel(const uint8_t unit, const uint8_t request, uint8_t *data, uint8_t *responseLength, const bool isRead) {
	uint8_t length = 0;
	sChannel *channels = NULL;
	const eChannelDataType channelDTReq = request >> 6;
    const uint8_t channelNumPerm = request & 0x3F;
    const uint8_t channelNum = channelNumPerm & 0x0F;
	*responseLength = 0;
    if (0x10 > channelNumPerm) {
		if (isRead) {
			length = pgm_read_byte(&(units[unit].channelsROLength));
			channels = pgm_read_ptr(&(units[unit].channelsRO));
		} else {
			return ercChBadPermissions;
		}
    } else if (0x20 > channelNumPerm) {
		if (! isRead) {
			length = pgm_read_byte(&(units[unit].channelsWOLength));
			channels = pgm_read_ptr(&(units[unit].channelsWO));
		} else {
			return ercChBadPermissions;
		}
    } else if (0x30 > channelNumPerm) {
		length = pgm_read_byte(&(units[unit].channelsRWLength));
		channels = pgm_read_ptr(&(units[unit].channelsRW));
	} else {
		return ercChBadChannels;
	}
	
    if (length <= channelNum) {
		return ercChBadChannels;
	}
    const eChannelDataType channelDT = pgm_read_byte((const uint8_t*) &(channels[channelNum].dataType));
    void *value = pgm_read_ptr(&(channels[channelNum].value));
	
	if (channelDTReq != channelDT) {
		return ercChValidationFailed;
	}
	
	// validation done
	if (isRead) {
		data[0] = channelDT << 6 | channelNumPerm;
		if (eCDTArray == channelDT) {
			//uint8_t arrayRawLength = _dataTypeLength[channelDT] + ((string*) value)->length;
			//memcpy(&(data[1]), value, arrayRawLength);
			//*responseLength = 1 + arrayRawLength;
		} else {
			memcpy(&(data[1]), value, _dataTypeLength[channelDT]);
			*responseLength = 1 + _dataTypeLength[channelDT];
		}
	} else {
		if (eCDTArray == channelDT) {
			//uint8_t arrayRawLength = _dataTypeLength[channelDT] + ((string*) data)->length;
			//memcpy(value, &(data[0]), arrayRawLength);
			//*responseLength = arrayRawLength;
		} else {
			memcpy(value, &(data[0]), _dataTypeLength[channelDT]);
			*responseLength = _dataTypeLength[channelDT];
		}
	}
	return ercOk;	
}

uint8_t readUnitChannel(const uint8_t unit, const scString *request, sString *response) {
    uint8_t length = 0;
    for (int i = 0; i < request->length; i++) {
		uint8_t valueLength = 0;
        uint8_t code = _readWriteSingleUnitChannel(unit, request->data[i], response->data + length, &valueLength, true);
		length += valueLength;
		
		if (ercOk != code) {
			// cancel and return error code instead
			response->length = 0;
			return code;
		}
	}
	response->length = length;
	return ercOk;
}

uint8_t writeUnitChannel(const uint8_t unit, const scString *request, sString *response) {
	response->length = 0;
	uint8_t length = 0;
	while (length < request->length) {
		uint8_t valueLength;
		uint8_t code = _readWriteSingleUnitChannel(unit, request->data[length], (uint8_t*) request->data + length + 1, &valueLength, false);
		length += 1 + valueLength;
		if (ercOk != code) return code;
	}
	return ercOk;
}

const PROGMEM fRFFunction RFFunctions[_eFCount] = {
	[eFSetSessionKey] = &setSessionKey,
	[eFGetListOfUnits] = &getListOfUnits,
	[eFSetAddress] = &setMACAddress,
	[eFGetStatistics] = &getStatistics,
    [eFResetTransactionId] = &resetTransactionId,
	[eFNOP] = &rfNOP,
	
	[eFGetPropertiesOfUnit] = &getPropertiesOfUnit,
	[eFGetTextDescription] = &getTextDescription,
	[eFSetTextDescription] = &setTextDescription,
	
	//[eFReadSingleUnitChannel] = &_readSingleUnitChannel,
	
    [eFReadUnitChannels] = &readUnitChannel,
    [eFWriteUnitChannels] = &writeUnitChannel,
};
