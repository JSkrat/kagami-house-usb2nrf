
#ifndef NRF_MODEL_H_
#define NRF_MODEL_H_


#include <stdint.h>
#include "sstring.h"
#include "../usb2nrf/defines.h"
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"

extern nRF24L01 *rfTransiever;

typedef void (*fNRFCallback)(void);
extern fNRFCallback cNRF_DataTransmitted, cNRF_DataReceived, cNRF_TransmissionFailed;

void nRF_init_for_tests();
void nRF_free_for_tests();

void nRF_init();


extern nRF24L01 *rfTransiever;
extern int listenCalls;
extern uint8_t llaData[256];
extern sString lastListenAddress;

#endif /* NRF_MODEL_H_ */
