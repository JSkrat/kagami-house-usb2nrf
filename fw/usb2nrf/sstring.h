/*
 * string.h
 *
 * Created: 13.04.2020 23:04:49
 *  Author: Mintytail
 */ 


#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

#pragma pack(1)
typedef struct {
	uint8_t length;
	uint8_t *data;
} sString;

typedef struct {
	const uint8_t length;
	const uint8_t *data;
} scString;
#pragma pack()

#endif /* STRING_H_ */
