/*
 * Settings.c
 *
 * Created: 16.05.2020 20:27:23
 *  Author: Mintytail
 */ 
#include "Settings.h"
#include "defines.h"
#include <avr/eeprom.h>
#include <stdint.h>

const static uint16_t settingsSize[esAmount] = {
	[esKey] = 256,
	[esAddress] = MAC_SIZE,
	[esChannel] = 1,
	[esMode] = 1,
};

const static uint16_t settingsAddress[esAmount] = {
	[esKey] = 256,
	[esAddress] = 8,
	[esChannel] = 0,
	[esMode] = 1,
};

uint8_t readSetting(eSetting type, void *output) {
	if (esAmount <= type) return 0;
	eeprom_read_block(output, (void*) settingsAddress[type], settingsSize[type]);
	return settingsSize[type];
}

uint8_t saveSettings(eSetting type, const void *data) {
	if (esAmount <= type) return 0;
	eeprom_update_block(data, (void*) settingsAddress[type], settingsSize[type]);
	return settingsSize[type];
}

