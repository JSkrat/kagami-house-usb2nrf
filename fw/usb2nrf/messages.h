
#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

#include <avr/pgmspace.h>

typedef struct {
	uint8_t length;
	uint8_t data[];
} string;

//#define cTerm    16
#define cCheck   94
#define cCelsius 96


extern const string m_hello PROGMEM;
extern const string m_sendQueueSize PROGMEM;
extern const string m_usError PROGMEM;
extern const string m_usStart PROGMEM;
extern const string m_usPAddressTo PROGMEM;
extern const string m_usPDataLength PROGMEM;
extern const string m_usPData PROGMEM;
extern const string m_uPacketLen PROGMEM;

#endif // MESSAGES_H_INCLUDED
