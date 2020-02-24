#include "gtest/gtest.h"

extern "C" {
    #include "functions.h"
    #include "defines.h"
    #include "RFparser.h"
}
#include "units_structure.h"

extern uint8_t *__start_progmem;
extern uint8_t *__stop_progmem;

namespace my {
namespace project {
namespace {

class FunctionsTest: public ::testing::Test {
protected:
    void invert_progmem() {
        /// here we're inverting the whole section so direct access will return not what is there, failing the test
        /// only allowed access is through pgm_* functions
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
    free(response);
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
    free(request);
    free(response);
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
    free(response);
}

TEST_F(FunctionsTest, TestGetPropertiesOfUnit) {
    // first let's get list of all units
    // assuming that function is already tested and working
    string *u_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
    ASSERT_EQ(C_OK, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, u_response));
    ASSERT_LT(0, u_response->length) << "list of units response is empty";
    ASSERT_LT(0, u_response->data[0]) << "there are no units in a given device";
//    for (int unit = 0; unit < 256; unit++) {
    for (uint8_t unit = 0; unit < u_response->data[0]; unit++) {
        string *p_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
        const uint8_t code = (*(this->getFunction(eFGetPropertiesOfUnit)))(unit & 0xFF, nullptr, p_response);
        // and now let's check the properties of unit
        if (unit < u_response->data[0]) {
            const uint8_t u_base = 1 + 4*(unit&0xFF);
            const uint8_t type = u_response->data[u_base];
            const uint8_t num_ro = u_response->data[u_base+1];
            const uint8_t num_wo = u_response->data[u_base+2];
            const uint8_t num_rw = u_response->data[u_base+3];
            EXPECT_EQ(C_OK, code);
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
        } else {
            // never happen, functions not checking for unit validity, it is in RFParser
            ASSERT_EQ(C_CH_BAD_CHANNELS, code);
        }
        free(p_response);
    }
    free(u_response);
}

TEST_F(FunctionsTest, TestReadSingleChannel) {
    // first let's get list of all units
    // assuming that function is already tested and working
    string *u_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
    ASSERT_EQ(C_OK, (*(this->getFunction(eFGetListOfUnits)))(0, nullptr, u_response));
    ASSERT_LT(0, u_response->length) << "list of units response is empty";
    ASSERT_LT(0, u_response->data[0]) << "there are no units in a given device";
    // test every channel of each unit
//    for (int unit = 0; unit < 256; unit++) {
    for (int unit = 0; unit < UNITS_LENGTH; unit++) {
        for (uint8_t channel = 0; channel < 0x3F; channel++) {
            const uint8_t channelPerm = channel >> 4;
            for (uint8_t chType = 0; chType < 4; chType++) {
                #define MSG "TestReadSingleChannel unit " << static_cast<int>(unit) << \
                    ", channel " << static_cast<int>(channel) << " (" << static_cast<int>(channelPerm) << ", " << \
                    static_cast<int>(channel & 0x0F) << "), testing type " << static_cast<int>(chType) << "\n"
                string *request = reinterpret_cast<string*>(malloc(RF_RESP_DATA_LENGTH));
                string *c_response = reinterpret_cast<string*>(malloc(sizeof(string) + RF_RESP_DATA_LENGTH));
                request->length = 1;
                request->data[0] = ((chType << 6) & 0xFF) | channel;
                uint8_t code = (*(this->getFunction(eFReadUnitChannels)))(unit & 0xFF, request, c_response);

                int testIndex = unit * CHANNELS_PER_UNIT + channel;
                if (channel < CHANNELS_PER_UNIT && testChannels.count(testIndex)) {
                    // test channel exists
                    const sChannel ch = testChannels.at(testIndex);
                    if (1 != channelPerm) {
                        if (ch.dataType == chType) {
                            ASSERT_EQ(C_OK, code) << MSG << "code not OK";
                            ASSERT_GT(c_response->length, 1) << MSG << "response length less than 2";
                            ASSERT_EQ(((chType << 6) & 0xFF) | channel, c_response->data[0]) << MSG << "response channel type not as requested";
                            switch (ch.dataType) {
                            case eCDTBit: {
                                bool should_be = *(ch.value.tBit);
                                bool is = *(reinterpret_cast<bool*>(&(c_response->data[1])));
                                EXPECT_EQ(should_be, is)
                                        << MSG << "boolean value not equals";
                                break;
                            }
                            case eCDTSigned: {
                                EXPECT_EQ(*(ch.value.tInt), *(reinterpret_cast<int32_t*>(&(c_response->data[1]))))
                                        << MSG << "int32 value not equals";
                                break;
                            }
                            case eCDTUnsigned: {
                                EXPECT_EQ(*(ch.value.tUInt), *(reinterpret_cast<uint32_t*>(&(c_response->data[1]))))
                                        << MSG << "uint32 value not equals";
                                break;
                            }
                            }
                        } else {
                            EXPECT_EQ(C_CH_VALIDATION_FAILED, code) << MSG << "code should be VALIDATION_FAILED";
                            EXPECT_EQ(0, c_response->length) << MSG << "response length should be 0 at VALIDATION FAILED";
                        }
                    } else {
                        EXPECT_EQ(C_CH_BAD_PERMISSIONS, code) << MSG << "code should be BAD PERMISSIONS";
                        EXPECT_EQ(0, c_response->length) << MSG << "response length should be 0 at BAD PERMISSIONS";
                    }
                } else {
                    if (UNITS_LENGTH > unit && 0x10 <= channel && 0x20 > channel) {
                        EXPECT_EQ(C_CH_BAD_PERMISSIONS, code) << MSG "code should be BAD PERMISSIONS";
                        EXPECT_EQ(0, c_response->length) << MSG << "response length should be 0 at BAD PERMISSIONS";
                    } else {
                        EXPECT_EQ(C_CH_BAD_CHANNELS, code) << MSG "code should be BAD CHANNELS";
                        EXPECT_EQ(0, c_response->length) << MSG << "response length should be 0 at BAD CHANNELS";
                    }
                }
                free(request);
                free(c_response);
            }
        }
    }
    free(u_response);
}

} //
} // project
} // my
