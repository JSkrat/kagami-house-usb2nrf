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

const static uint8_t settingsSize[esAmount] = {
	[esKey] = 256,
	[esAddress] = MAC_SIZE,
	[esChannel] = 1,
};

const static void* settingsAddress[esAmount] = {
	[esKey] = 256,
	[esAddress] = 1,
	[esChannel] = 0,
};

uint8_t readSetting(eSetting type, void *output) {
	if (esAmount <= type) return 0;
	eeprom_read_block(output, settingsAddress[type], settingsSize[type]);
	return settingsSize[type];
}

uint8_t saveSettings(eSetting type, const void *data) {
	if (esAmount <= type) return 0;
	eeprom_update_block(data, settingsAddress[type], settingsSize[type]);
	return settingsSize[type];
}

