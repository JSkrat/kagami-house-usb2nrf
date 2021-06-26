/*
 * RF_custom_functions.h
 *
 * Created: 23.05.2020 16:53:12
 *  Author: Mintytail
 */ 


#ifndef RF_CUSTOM_FUNCTIONS_H_
#define RF_CUSTOM_FUNCTIONS_H_

#include "../KagamiCore/RF functions.h"

#define unitsCount 1
extern const tUnit CRFUnits[unitsCount];

#define fU1Count 12
extern const tRFCodeFunctionItem U1Functions[fU1Count];

void RFCustomFunctionsInit();

#endif /* RF_CUSTOM_FUNCTIONS_H_ */
