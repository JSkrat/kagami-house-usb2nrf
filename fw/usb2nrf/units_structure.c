/*
 * units_structure.c
 *
 * Created: 09.02.2020 16:35:19
 *  Author: Mintytail
 */ 
#include "units_structure.h"
#include "RF protocol.h"


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
