
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
extern const string m_last_command PROGMEM;
extern const string m_c_none PROGMEM;
extern const string m_c_status PROGMEM;
extern const string m_c_receive PROGMEM;
extern const string m_c_send PROGMEM;

#endif // MESSAGES_H_INCLUDED
