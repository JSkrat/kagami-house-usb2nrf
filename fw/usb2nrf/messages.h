
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
extern const string m_mGHIntegral PROGMEM;
extern const string m_mGHProportional PROGMEM;
extern const string m_mGHDerivative PROGMEM;
extern const string m_mMode PROGMEM;
extern const string m_mMenuTimeout PROGMEM;
extern const string m_mMenuShutdown PROGMEM;
extern const string m_mDebug PROGMEM;
extern const string m_smInc PROGMEM;
extern const string m_smDec PROGMEM;
extern const string m_smInc10 PROGMEM;
extern const string m_smDec10 PROGMEM;
extern const string m_smCancel PROGMEM;

#endif // MESSAGES_H_INCLUDED
