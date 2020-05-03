/*
 * nRF model.h
 * low level interface to the nrf library and some supplementary objects and structures
 *
 * Created: 05.03.2020 21:51:57
 *  Author: Mintytail
 */ 


#ifndef NRF_MODEL_H_
#define NRF_MODEL_H_


#include <stdint.h>
#include "sstring.h"
#include "../usb2nrf/defines.h"
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"

extern nRF24L01 *rfTransiever;

typedef void (*fNRFCallback)(sString *address, sString *payload);
extern fNRFCallback cNRF_DataTransmitted, cNRF_DataReceived, cNRF_TransmissionFailed;

void nRF_init();
void nRF_listen(const uint8_t *address);
void nRF_transmit(uint8_t *address, uint8_t length, uint8_t *data);

#endif /* NRF_MODEL_H_ */
