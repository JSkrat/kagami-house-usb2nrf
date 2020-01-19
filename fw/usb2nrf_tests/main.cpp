#include "gtest/gtest.h"

extern "C" {
    #include "functions.h"
    #include "defines.h"
    #include "RFparser.h"
}

extern uint8_t *__start_progmem;
extern uint8_t *__stop_progmem;

namespace my {
namespace project {
namespace {

class FunctionsTest: public ::testing::Test {
protected:
    void invert_progmem() {
        uint8_t *iByte = reinterpret_cast<uint8_t*>(&__start_progmem);
        while (iByte < reinterpret_cast<uint8_t*>(&__stop_progmem)) {
            *iByte = ~(*iByte);
            iByte++;
        }
    }

    fRFFunction getFunction(eFunctions from) {
        return reinterpret_cast<fRFFunction>(
                    pgm_read_ptr(
                        const_cast<void*>(
                            reinterpret_cast<const void*>(&(RFFunctions[from]))
                        )
                    )
        );
    }

    FunctionsTest() {
        // set-up work for each test
        // invert the whole progmem section so direct reading will return something wrong
        this->invert_progmem();
    }

    ~FunctionsTest() override {
        // clean-up work for each test
        this->invert_progmem();
    }
};

/*TEST_F(FunctionsTest, Test_session_key) {
    int ret = (*(this->getFunction(eFSetSessionKey)))(0, nullptr, nullptr);
    EXPECT_EQ(C_NOT_IMPLEMENTED, ret);
}//*/

TEST_F(FunctionsTest, TestListOfUnits) {
    string *response = reinterpret_cast<string*>(malloc(sizeof(string) + 1030));
    ASSERT_EQ(C_OK, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, response));
    // we assume we do not know what was initialized, let's only check the validity, so any data set could be tested
    ASSERT_GE(RF_RESP_DATA_LENGTH, response->length) << "response too big" << response->length;
    ASSERT_LE(1, response->length) << "response is empty";
    uint8_t units = response->data[0];
    ASSERT_LE(1, units) << "there should not be devices without units at all";
    // and check units one by one
    for (uint8_t u = 0; u < units; u++) {
        uint8_t base = 1 + u*4;
        ASSERT_GE(RF_RESP_DATA_LENGTH, base+4) << "response does not fit all units:" << units;
        uint8_t type = response->data[base];
        EXPECT_TRUE(type < eUTSLast || ((type > eUTACustom) && (type < eUTALast))) << "wrong unit type" << type;
        EXPECT_TRUE(
                    (response->data[base+1] > 0) ||
                    (response->data[base+2] > 0) ||
                    (response->data[base+3] > 0)
                ) << "unit can not be without any channel";
        // 16 channels max per access_type
        EXPECT_GT(16, response->data[base+1]);
        EXPECT_GT(16, response->data[base+2]);
        EXPECT_GT(16, response->data[base+3]);
    }
}

TEST_F(FunctionsTest, TestSetMacAddress) {
    string *response = reinterpret_cast<string*>(malloc(sizeof(string) + 256));
    string *request = reinterpret_cast<string*>(malloc(sizeof(string) + MAC_SIZE+1));
    auto testMac = [request, response, this](uint8_t mac_length, int result) {
        request->length = mac_length;
        response->length = 0; // default is 0
        EXPECT_EQ(result, (*(this->getFunction(eFSetAddress)))(0, request, response));
        EXPECT_EQ(0, response->length);
    };
    testMac(MAC_SIZE, C_OK);
    testMac(0, C_AD_BAD_LENGTH);
    testMac(MAC_SIZE - 1, C_AD_BAD_LENGTH);
    testMac(MAC_SIZE + 1, C_AD_BAD_LENGTH);
}

TEST_F(FunctionsTest, TestGetRFStatistics) {
    string *response = reinterpret_cast<string*>(malloc(sizeof(string) + 256));
    total_requests = 0xFEDC;
    ok_responses = 0xEDCB;
    error_responses = 0xCDBA;
    transaction_errors = 0xBCA9;
    ack_timeouts = 0xAB98;
    ASSERT_EQ(C_OK, (*(this->getFunction(eFGetRFStatistics)))(0, nullptr, response));
    typedef struct {
        uint16_t requests;
        uint16_t responses;
        uint16_t errors;
        uint16_t trans_errors;
        uint16_t ack_to;
    } tRFStatResponse;
    ASSERT_EQ(sizeof(tRFStatResponse), response->length);
    tRFStatResponse *parsedResponse = reinterpret_cast<tRFStatResponse *>(&(response->data));
    EXPECT_EQ(total_requests, parsedResponse->requests);
    EXPECT_EQ(ok_responses, parsedResponse->responses);
    EXPECT_EQ(error_responses, parsedResponse->errors);
    EXPECT_EQ(transaction_errors, parsedResponse->trans_errors);
    EXPECT_EQ(ack_timeouts, parsedResponse->ack_to);
}

TEST_F(FunctionsTest, TestGetPropertiesOfUnit) {
    // first let's get list of all units
    // assuming that function is already tested and working
    string *u_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
    ASSERT_EQ(C_OK, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, u_response));
    ASSERT_LT(0, u_response->length) << "list of units response is empty";
    ASSERT_LT(0, u_response->data[0]) << "there are no units in a given device";
    for (uint8_t u = 0; u < u_response->data[0]; u++) {
        const uint8_t u_base = 1 + 4*u;
        const uint8_t type = u_response->data[u_base];
        const uint8_t num_ro = u_response->data[u_base+1];
        const uint8_t num_wo = u_response->data[u_base+2];
        const uint8_t num_rw = u_response->data[u_base+3];
        // and now let's check the properties of unit
        string *p_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
        EXPECT_EQ(C_OK, (*(this->getFunction(eFGetPropertiesOfUnit)))(u, nullptr, p_response));
        ASSERT_EQ(1 + 4*3, p_response->length) << "response length should be 1 + 4x3 bytes long" << p_response->length;
        EXPECT_EQ(type, p_response->data[0]);
        // at least we could check if non-existent channels have type 00
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (num_ro <= i*4+j) {
                    EXPECT_EQ(0, (p_response->data[1+(i*1)] >> (j*2)) & 0b11);
                }
                if (num_wo <= i*4+j) {
                    EXPECT_EQ(0, (p_response->data[1+(i*2)] >> (j*2)) & 0b11);
                }
                if (num_rw <= i*4+j) {
                    EXPECT_EQ(0, (p_response->data[1+(i*3)] >> (j*2)) & 0b11);
                }
            }
        }
    }
}

TEST_F(FunctionsTest, TestReadSingleChannel) {
    // this we're testing accross all units and all channels
    // but we need a hash table
    string *request = reinterpret_cast<string*>(malloc(sizeof(string) + 1));
    request->length = 1;

}

} //
} // project
} // my
