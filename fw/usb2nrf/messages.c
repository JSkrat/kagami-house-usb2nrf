#include "messages.h"

const string m_hello PROGMEM = { 15, { 'K', 'y', 'c', 196, ' ','M','i','n','e','O','z','e','l','o','t', } };
const string m_mGHIntegral PROGMEM = { 8, { 'P','I','D',' ','G','H',' ','I', } };
const string m_mGHProportional PROGMEM = { 8, { 'P','I','D',' ','G','H',' ','P', } };
const string m_mGHDerivative PROGMEM = { 8, { 'P','I','D',' ','G','H',' ','D', } };
const string m_mMode PROGMEM = { 14, { 'H','e','a','t',' ','f','e','e','d','b','a','c','k',0x20,  } };
const string m_mMenuTimeout PROGMEM = { 13, { 'M','e','n','u',' ','t','i','m','e','o','u','t',0x20,  } };
const string m_mMenuShutdown PROGMEM = { 13, { 'S','h','u','t','d','o','w','n',' ','a','i','r',0x20,  } };
const string m_mDebug PROGMEM = { 11, { 'D','e','b','u','g',' ','i','n','f','o',0x20,  } };
const string m_smInc PROGMEM = { 2, { '+','1', } };
const string m_smDec PROGMEM = { 2, { '-','1', } };
const string m_smInc10 PROGMEM = { 3, { '+','1','0', } };
const string m_smDec10 PROGMEM = { 3, { '-','1','0', } };
const string m_smCancel PROGMEM = { 6, { 'c','a','n','c','e','l', } };
