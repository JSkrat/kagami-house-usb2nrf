#include "messages.h"

const string m_hello PROGMEM = { 16, { 0x20, 'k','a','g','a','m','i',' ','u','s','b','2','n','r','f',0x20,  } };
const string m_sendQueueSize PROGMEM = { 8, { 'S','Q','u','e','u','e',':',0x20,  } };
const string m_usError PROGMEM = { 6, { 'e','r','r','o','r',0x20,  } };
const string m_usStart PROGMEM = { 6, { 's','t','a','r','t',0x20,  } };
const string m_usPAddressTo PROGMEM = { 6, { 'a','d','d','r',0x20, 0x20,  } };
const string m_usPDataLength PROGMEM = { 6, { 'l','e','n',0x20, 0x20, 0x20,  } };
const string m_usPData PROGMEM = { 6, { 'd','a','t','a',0x20, 0x20,  } };
const string m_uPacketLen PROGMEM = { 9, { 'p','a','c','k',' ','l','e','n',0x20,  } };
