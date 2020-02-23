#include "tests.h"
#include <stdint.h>
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/nrf24l01-mnemonics.h"

uint8_t qwerty PROGMEM = 0;

void *pgm_read_ptr_near(void *ptr) {
//    return *((void **)ptr);
    return pgm_read_ptr(ptr);
}

void *pgm_read_ptr(void *ptr) {
    return (void *)(~(*(uint64_t*)(ptr)));
}

uint8_t pgm_read_byte(uint8_t *ptr) {
    return ~(*ptr);
}

int nRF24L01_transmit_success(nRF24L01 *rf) {
    return 0;
}

void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg) {

}

uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t regAddress, void *data,
                               size_t length) {
    return 0;
}

bool nRF24L01_data_received(nRF24L01 *rf) {
    return false;
}

bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message) {
    return true;
}

void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address) {

}

void uSendPacket(union uPackage *packet) {

}
