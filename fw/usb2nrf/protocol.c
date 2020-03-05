/*
 * protocol.c
 *
 * Created: 01.03.2020 22:24:02
 *  Author: Mintytail
 */ 

#include "../usb2nrf/protocol.h"
#include "../usb2nrf/functions.h"
#include "units_structure.h"
#include <stdint.h>
#include <string.h> // for NULL

uint8_t lastReceivedTransacrionId;


enum eResponseCodes lastSentPacketStatus;


enum eResponseCodes validatePacket(uint8_t length, uint8_t *data) {
	// first lets check the packet minimum length
	if (eqoData > length) return ercBadRequestData;
	// check the contents
	if (0 != data[eqoVersion]) return ercBadVersion;
	if (lastReceivedTransacrionId+1 != data[eqoTransactionId]) {
		return ercNotConsecutiveTransactionId;
	}
	// only basic check if unit 0 functions called for not unit 0 and vice versa
	if ((0 == data[eqoUnitId]) != (0x10 > data[eqoFunctionId])) {
		return ercBadFunctionId;
	}
	if (_eFCount <= data[eqoFunctionId]) return ercBadFunctionId;
	// check if unit exists
	if (UNITS_LENGTH <= data[eqoUnitId]) {
		return ercBadUnitId;
	}
	return ercOk;
}

void generateResponse(uint8_t requestLength, uint8_t *requestData, uint8_t *responseLength, uint8_t *responseData) {
	// response should be already allocated for that function
	enum eResponseCodes validation = validatePacket(requestLength, requestData);
	responseData[eroVersion] = PROTOCOL_VERSION;
	responseData[eroTransactionId] = requestData[eqoTransactionId];
	responseData[eroCode] = validation;
	switch (validation) {
		case ercOk: {
			fRFFunction method = pgm_read_ptr(RFFunctions[requestData[eqoFunctionId]]);
			if (NULL == method) {
				responseData[eroCode] = ercBadFunctionId;
				break;
			}
			sString requestArg = {
				.length = requestLength - eqoData,
				.data = &requestData[eqoData]
			};
			sString responseArg = {
				.length = 0,
				.data = &responseData[eroData]
			};
			responseData[eroCode] = (*method)(
				requestData[eqoUnitId],
				&requestArg,
				&responseArg
			);
			*responseLength = responseArg.length;
			break;
		}
		case ercNotConsecutiveTransactionId: {
			responseData[eroData] = lastReceivedTransacrionId;
		}
		default: break;
	}
}

void protocolInit() {
	lastReceivedTransacrionId = 0;
}
