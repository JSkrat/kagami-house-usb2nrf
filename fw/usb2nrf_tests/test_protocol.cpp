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

    ProtocolTest() {
        // set-up work for each test
        reinitialize_memory();
        protocolInit();
        this->request = reinterpret_cast<uint8_t*>(malloc(256));
        this->response = reinterpret_cast<uint8_t*>(malloc(256));
        memset(this->request, 0, 256);
        RQ->rqVersion = PROTOCOL_VERSION;
        RQ->rqFunctionId = eFNOP;
        memset(this->response, 0, 256);
        responseLength = 0;
    }

    ~ProtocolTest() override {
        // clean-up work for each test
        free(this->request);
        free(this->response);
    }
};

TEST_F(ProtocolTest, TestProtocolVersion) {
    for (int i = 0; i < 256; i++) {
        RQ->rqVersion = static_cast<uint8_t>(i);
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
        if (PROTOCOL_VERSION == i)
            ASSERT_EQ(ercOk, RS->rsCode) << "not ok response code for correct protocol version";
        else
            ASSERT_EQ(ercBadVersion, RS->rsCode) << "wrong response code for incorrect protocol version";

    }
}

TEST_F(ProtocolTest, TestRequestLength) {
    // build a correct request, but wrong length
    for (uint8_t i = 0; i < REQUEST_HEADER_SIZE; i++) {
        generateResponse(i, this->request, &this->responseLength, this->response);
        ASSERT_EQ(ercBadRequestData, RS->rsCode) << "wrong result code for request length " << i;
    }
}

TEST_F(ProtocolTest, TestTransactionId) {
    // first obtain current transaction id
    uint8_t transactionId = RS->rsData[0];
    // positive test for correct transaction id changes with overflow
    for (unsigned int i = 0; i < 256; i++) {
        RQ->rqVersion = PROTOCOL_VERSION;
        RQ->rqTransactionId = static_cast<uint8_t>(i);
        RQ->rqFunctionId = eFNOP;
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
        ASSERT_EQ(ercOk, RS->rsCode) << "not ok response " << static_cast<unsigned int>(RS->rsCode)
                                     << "at transaction id " << i;
        ASSERT_EQ(i, RS->rsTransactionId) << "transaction id not equal to given one for correct request"
                                          << "at transaction id " << i
                                          << "returned " << static_cast<unsigned int>(RS->rsTransactionId);
        // check for bad request. if it works for wrong protocol version, it hopefully will work for all errors
        RQ->rqVersion = PROTOCOL_VERSION + 1;
        generateResponse(REQUEST_HEADER_SIZE, this->request, &this->responseLength, this->response);
        ASSERT_EQ(i, RS->rsTransactionId) << "transaction id not equal to given one for incorrect request"
                                          << "at transaction id " << i
                                          << "returned " << static_cast<unsigned int>(RS->rsTransactionId);
    }
}


} //
} // project
} // my
