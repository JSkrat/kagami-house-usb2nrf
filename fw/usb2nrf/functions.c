/*
 * functions.c
 *
 * Created: 01.01.2020 14:54:34
 *  Author: Mintytail
 */ 

#include "functions.h"

#ifndef UNIT_TESTING
    #include "messages.h"
    #include <avr/pgmspace.h>
#else

#endif
#include "RF parser.h"
#include <stdbool.h>
#include <string.h>

bool switches[5];
bool relays[2];
uint16_t analog_output;

const sChannel switchChannels[] PROGMEM = {
	{
		.dataType = eCDTBit,
		.value = {&switches[0]}
	},
	{
		.dataType = eCDTBit,
		.value = {&switches[1]}
	},
	{
		.dataType = eCDTBit,
		.value = {&switches[2]}
	},
	{
		.dataType = eCDTBit,
		.value = {&switches[3]}
	},
	{
		.dataType = eCDTBit,
		.value = {&switches[4]}
	},
};

const sChannel outputChannels[] PROGMEM = {
	{
		.dataType = eCDTBit,
		.value = {&relays[0]}
	},
	{
		.dataType = eCDTBit,
		.value = {&relays[1]}
	},
	{
		.dataType = eCDTUnsigned,
		.value = {.tUInt = (uint32_t*) &analog_output}
	},
};

const sUnit units[UNITS_LENGTH] PROGMEM = {
	{
		.type = eUTSSwitch,
		.channelsROLength = 1,
		.channelsRO = &switchChannels[0]
	},
	{
		.type = eUTSSwitch,
		.channelsROLength = 1,
		.channelsRO = &switchChannels[1]
	},
	{
		.type = eUTSCustom,
		.channelsROLength = 3,
		.channelsRO = &switchChannels[2]
	},
	{
		.type = eUTARelay,
		.channelsRWLength = 1,
		.channelsRW = &outputChannels[0]
	},
	{
		.type = eUTARelay,
		.channelsRWLength = 1,
		.channelsRW = &outputChannels[1]
	},
	{
		.type = eUTAAnalog,
		.channelsRWLength = 1,
		.channelsRW = &outputChannels[2]
	}
};

const uint8_t _dataTypeLength[] = {
	[eCDTBit] = 1,
	[eCDTUnsigned] = 4,
	[eCDTSigned] = 4,
	[eCDTArray] = 1,
};

uint8_t setSessionKey(uint8_t unit, string *request, string *response) {
	// TODO after integrating cipher library
	return C_NOT_IMPLEMENTED;
}

uint8_t getListOfUnits(uint8_t unit, string *request, string *response) {
    (void) unit;
    (void) request;
	response->length = 1 + 4*UNITS_LENGTH;
	response->data[0] = UNITS_LENGTH;
	for (int i = 0; i < UNITS_LENGTH; i++) {
		uint8_t base = 1 + 4*i;
        response->data[base+0] = pgm_read_byte(&(units[i].type));
        response->data[base+1] = pgm_read_byte(&(units[i].channelsROLength));
        response->data[base+2] = pgm_read_byte(&(units[i].channelsWOLength));
        response->data[base+3] = pgm_read_byte(&(units[i].channelsRWLength));
	}
	return C_OK;
}

uint8_t setMACAddress(uint8_t unit, string *request, string *response) {
    (void) unit;
    (void) response;
//    response->length = 0;
	if (5 == request->length) {
		setListenAddress((t_address*) request->data);	
		return C_OK;
	} else {
		return C_AD_BAD_LENGTH;
	}
}

uint8_t getRFStatistics(uint8_t unit, string *request, string *response) {
    (void) unit;
    (void) request;
	response->length = 10;
    #define STORE_16(index, value) response->data[index] = value & 0xFF; response->data[index+1] = ((uint16_t)value) >> 8
	STORE_16(0, total_requests);
	STORE_16(2, ok_responses);
	STORE_16(4, error_responses);
	STORE_16(6, transaction_errors);
	STORE_16(8, ack_timeouts);
	return C_OK;
}

uint8_t getPropertiesOfUnit(uint8_t unit, string *request, string *response) {
	response->length = 13;
    response->data[0] = pgm_read_byte(&(units[unit].type));
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
                ro |= pgm_read_byte(&(ro_ch[index].dataType));
            if (wo_num > index)
                wo |= pgm_read_byte(&(wo_ch[index].dataType));
            if (rw_num > index)
                rw |= pgm_read_byte(&(rw_ch[index].dataType));
		}
		response->data[1 + i + 4*0] = ro;
		response->data[1 + i + 4*1] = wo;
		response->data[1 + i + 4*2] = rw;
	}
	return C_OK;
}

uint8_t getTextDescription(uint8_t unit, string *request, string *response) {
	return C_NOT_IMPLEMENTED;
}

uint8_t setTextDescription(uint8_t unit, string *request, string *response) {
	return C_NOT_IMPLEMENTED;
}

uint8_t _readSingleUnitChannel(uint8_t unit, uint8_t request, uint8_t *response, uint8_t *responseLength) {
	uint8_t length = 0;
	sChannel *channels = NULL;
	//if (1 != request->length) { return C_BAD_REQUEST_DATA; }
	const eChannelDataType channelDTReq = request >> 6;
	const uint8_t channelNumReq = request & 0x3F;
	*responseLength = 0;
	if (0x10 > channelNumReq) {
		length = pgm_read_byte(&(units[unit].channelsROLength));
		channels = pgm_read_ptr(&(units[unit].channelsRO));
	} else if (0x20 > channelNumReq) {
		return C_CH_BAD_PERMISSIONS;
	} else if (0x30 > channelNumReq) {
		length = pgm_read_byte(&(units[unit].channelsRWLength));
		channels = pgm_read_ptr(&(units[unit].channelsRW));
	} else {
		return C_CH_BAD_CHANNELS;
	}
	
	if (length <= channelNumReq) {
		return C_CH_BAD_CHANNELS;
	}
	const eChannelDataType channelDT = pgm_read_byte(&(channels[channelNumReq].dataType));
	const void *value = pgm_read_ptr(&(channels[channelNumReq].value));
	
	if (channelDTReq != channelDT) {
		return C_CH_VALIDATION_FAILED;
	}
	
	// validation done, fill up response
	response[0] = channelDT << 6 | channelNumReq;
	if (eCDTArray == channelDT) {
		uint8_t arrayLength = _dataTypeLength[channelDT] + ((string*) value)->length;
		memcpy(
			&(response[1]), 
			value, 
			arrayLength
		);
		*responseLength = 1 + arrayLength;
	} else {
		memcpy(&(response[1]), value, _dataTypeLength[channelDT]);
		*responseLength = 1 + _dataTypeLength[channelDT];
	}
	return C_OK;	
}

uint8_t readUnitChannel(uint8_t unit, string *request, string *response) {
	uint8_t length = 1;
	// copy count
	response->data[0] = request->data[0];
	for (int i = 0; i < request->data[0]; i++) {
		uint8_t valueLength = 0;
		uint8_t code = _readSingleUnitChannel(unit, request->data[1 + i], (&response->data[length]), &valueLength);
		length += valueLength;
		
		if (C_OK != code) {
			// cancel and return error code instead
			response->length = 0;
			return code;
		}
	}
	response->length = length;
	return C_OK;
}

uint8_t writeUnitChannel(uint8_t unit, string *request, string *response) {
	return 0;
}

const PROGMEM fRFFunction RFFunctions[_eFCount] = {
	[eFSetSessionKey] = &setSessionKey,
	[eFGetListOfUnits] = &getListOfUnits,
	[eFSetAddress] = &setMACAddress,
	[eFGetRFStatistics] = &getRFStatistics,
	
	[eFGetPropertiesOfUnit] = &getPropertiesOfUnit,
	[eFGetTextDescription] = &getTextDescription,
	[eFSetTextDescription] = &setTextDescription,
	
    [eFReadUnitChannels] = &readUnitChannel,
    [eFWriteUnitChannels] = &writeUnitChannel,
    [eFReadSingleUnitChannel] = &_readSingleUnitChannel,
};
