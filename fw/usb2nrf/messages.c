#include "messages.h"

const string m_hello PROGMEM = { 16, { 0x20, 'k','a','g','a','m','i',' ','u','s','b','2','n','r','f',0x20,  } };
const string m_sendQueueSize PROGMEM = { 8, { 'S','Q','u','e','u','e',':',0x20,  } };
const string m_last_command PROGMEM = { 9, { 'L','a','s','t','C','o','m','m',0x20,  } };
const string m_c_none PROGMEM = { 2, { '-','-', } };
const string m_c_status PROGMEM = { 7, { 's','t','a','t','u','s',0x20,  } };
const string m_c_receive PROGMEM = { 7, { 'r','e','c','e','i','v','e', } };
const string m_c_send PROGMEM = { 7, { 's','e','n','d',0x20, 0x20, 0x20,  } };
