/*
 * protocol.c
 *
 * Created: 01.03.2020 22:24:02
 *  Author: Mintytail
 */ 
#if BT_SLAVE == BUILD_TYPE

#include "../usb2nrf/RF protocol.h"
#include "../usb2nrf/RF functions.h"
#ifndef UNIT_TESTING
    #include "../usb2nrf/units_structure.h"
#else
    #include "../usb2nrf_tests/units_structure.h"
    #include "../usb2nrf_tests/pgmspace.h"
#endif
#include <stdint.h>
#include <string.h> // for NULL
#include <stddef.h> // for offsetof
#include "../usb2nrf/RF protocol internal.h"
#include "../usb2nrf/RF info.h"


//static enum eResponseCodes lastSentPacketStatus;


enum eResponseCodes validatePacket(const uint8_t length, const sRequest *data) {
	// first lets check the packet minimum length
	if (offsetof(sRequest, rqData) > length) return ercBadRequestData;
	// check the contents
	if (PROTOCOL_VERSION != data->rqVersion) return ercBadVersion;
	/*if (lastTransacrionId+1 != data->rqTransactionId) {
		// do not check transaction id if it is reset transaction id
		if (eFResetTransactionId != data->rqFunctionId)
			return ercNotConsecutiveTransactionId;
	}*/
	// check if unit exists
	if (UNITS_LENGTH <= data->rqUnitId) {
		return ercBadUnitId;
	}
	// only basic check if unit 0 functions called for not unit 0 and vice versa
	if ((0 == data->rqUnitId) != (0x10 > data->rqFunctionId)) {
		return ercBadFunctionId;
	}
	if (_eFCount <= data->rqFunctionId) return ercBadFunctionId;
	return ercOk;
}

void generateResponse(const uint8_t requestLength, const uint8_t *requestData, uint8_t *responseLength, uint8_t *responseData) {
	// response should be already allocated for that function
	#define REQUEST_DATA ((const sRequest*) requestData)
	#define RESPONSE_DATA ((sResponse*) responseData)
	enum eResponseCodes validation = validatePacket(requestLength, REQUEST_DATA);
	RESPONSE_DATA->rsVersion = PROTOCOL_VERSION;
	RESPONSE_DATA->rsTransactionId = REQUEST_DATA->rqTransactionId;
    RESPONSE_DATA->rsCode = (uint8_t) validation;
	*responseLength = 3; // length of the empty response, without any data
	switch (validation) {
		case ercOk: {
            fRFFunction method = (fRFFunction) pgm_read_ptr(&(RFFunctions[REQUEST_DATA->rqFunctionId]));
			if (NULL == method) {
				RESPONSE_DATA->rsCode = ercBadFunctionId;
				break;
			}
			const scString requestArg = {
				.length = requestLength - offsetof(sRequest, rqData),
				.data = &(REQUEST_DATA->rqData[0])
			};
			sString responseArg = {
				.length = 0,
				.data = &(RESPONSE_DATA->rsData[0])
			};
			RESPONSE_DATA->rsCode = (*method)(
				REQUEST_DATA->rqUnitId,
				&requestArg,
				&responseArg
			);
			*responseLength += responseArg.length;
			// update statistics
            if (0x80 > RESPONSE_DATA->rsCode) ok_responses++;
			else error_responses++;
			break;
		}
		/*case ercNotConsecutiveTransactionId: {
			missed_packets++;
			RESPONSE_DATA->rsData[0] = lastTransacrionId;
		}*/
		default: {
			validation_errors++;
			break;
		}
	}
	#undef RESPONSE_DATA
	#undef REQUEST_DATA
}

void generateAdvertisement(uint8_t *packetLength, uint8_t *packetData) {
	#define DATA ((sResponse*) packetData)
	fRFFunction method = (fRFFunction) pgm_read_ptr(&(RFFunctions[eFGetListOfUnits]));
	sString responseArg = {
		.length = 0,
		.data = &(DATA->rsData[0])
	};
	DATA->rsVersion = PROTOCOL_VERSION;
	DATA->rsTransactionId = 0;
	DATA->rsCode = (*method)(0, NULL, &responseArg);
	*packetLength = 3 + responseArg.length;
	#undef DATA
}

void protocolInit() {
	lastTransacrionId = 0;
}

#endif
