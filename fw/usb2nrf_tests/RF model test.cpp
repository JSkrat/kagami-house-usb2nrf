#include "gtest/gtest.h"
#include "RF model test.h"
#include "defines.h"
extern "C" {
    #include "nRF model.h"
    #include "../usb2nrf/RF model.h"
}
#include <exception>


namespace my {
namespace project {
namespace {


RFModelTest::RFModelTest()
{
//    reinitialize_memory();
//    nRF_init_for_tests();
//    lastListenAddress.data = new uint8_t[MAC_SIZE];
//    *(lastListenAddress.data) = 0xCE;
    rf_init();
}

RFModelTest::~RFModelTest()
{
//    nRF_free_for_tests();
//    delete[] lastListenAddress.data;
}

TEST_F(RFModelTest, listen_test) {
    t_address a;
    for (int i = 0; i < MAC_SIZE; i++) a[i] = static_cast<uint8_t>(i);
    listenCalls = 0;
    RFListen(&a);
    ASSERT_EQ(1, listenCalls) << "there should've been one call to nrf listen command in RFListen function";
    ASSERT_EQ(MAC_SIZE, lastListenAddress.length);
    for (int i = 0; i < MAC_SIZE; i++) {
        uint8_t *naddr = lastListenAddress.data;
        ASSERT_EQ(a[i], *naddr)
            << "listen address in command to nrf does not match given to RFListen function";
    }
}

TEST_F(RFModelTest, buffer_test) {
    ASSERT_EQ(nullptr, nextRFBufferElement()) << "buffer shall be empty right after init";
}

}}}
