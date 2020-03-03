
#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

#ifndef UNIT_TESTING
    #include <avr/pgmspace.h>
#else
    #include "../usb2nrf_tests/tests.h"
#endif
#include <stdint.h>

typedef struct {
	uint8_t length;
	uint8_t data[];
} string;

//#define cTerm    16
#define cCheck   94
#define cCelsius 96

#ifndef UNIT_TESTING
extern const string m_hello PROGMEM;
extern const string m_sendQueueSize PROGMEM;
extern const string m_usNotInit PROGMEM;
extern const string m_usErrorProto PROGMEM;
extern const string m_usErrorCmd PROGMEM;
extern const string m_usErrorLength PROGMEM;
extern const string m_usErrorData	 PROGMEM;
extern const string m_usProtoVer PROGMEM;
extern const string m_usCommand PROGMEM;
extern const string m_usPDataLength PROGMEM;
extern const string m_usPData PROGMEM;
extern const string m_usEnd PROGMEM;
extern const string m_uPacketLen PROGMEM;
extern const string m_rEN_RXADDR PROGMEM;

#endif
#endif // MESSAGES_H_INCLUDED