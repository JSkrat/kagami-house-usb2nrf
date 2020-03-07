#include "gtest/gtest.h"
extern "C" {
    #include "pgmspace.h"
    #include "../usb2nrf/functions.h"
    #include "../usb2nrf/protocol.h"
    #include "../usb2nrf/defines.h"
    #include "../usb2nrf/RF statistics.h"
}
#include "units_structure.h"
#include <string.h>
#include <stddef.h>

namespace my {
namespace project {
namespace {



class ProtocolTest: public ::testing::Test {
protected:
    uint8_t *request, *response, responseLength;
#define RQ (reinterpret_cast<sRequest*>(this->request))
#define RS (reinterpret_cast<sResponse*>(this->response))
    /*fRFFunction getFunction(eFunctions from) {
        return reinterpret_cast<fRFFunction>(
                    pgm_read_ptr(
                        const_cast<void*>(
                            reinterpret_cast<const void*>(&(RFFunctions[from]))
                        )
                    )
        );
    }*/

    ProtocolTest() {
        // set-up work for each test
        reinitialize_memory();
        protocolInit();
        request = reinterpret_cast<uint8_t*>(malloc(256));
        response = reinterpret_cast<uint8_t*>(malloc(256));
        memset(response, 0, 256);
        RQ->rqVersion = PROTOCOL_VERSION;
        responseLength = 0;
    }

    ~ProtocolTest() override {
        // clean-up work for each test
        free(request);
        free(response);
    }
};

TEST_F(ProtocolTest, Test_request_length) {
    // build a correct request, but wrong length
    for (uint8_t i = 0; i < REQUEST_HEADER_SIZE; i++) {
        generateResponse(i, this->request, &this->responseLength, this->response);
        ASSERT_EQ(ercBadRequestData, RS->rsCode) << "wrong result code for request length " << i;
    }
}//*/

TEST_F(ProtocolTest, Test_transaction_id) {
    // first obtain current transaction id
    RQ->rqFunctionId = eFResetTransactionId;
    generateResponse(REQUEST_HEADER_SIZE, this->request, &(this->responseLength), this->response);


}


} //
} // project
} // my
