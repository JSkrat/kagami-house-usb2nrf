/*
 * RF_info.h
 *
 * Created: 20.04.2020 21:38:32
 *  Author: Mintytail
 */ 


#ifndef RF_INFO_H_
#define RF_INFO_H_

#include <stdint.h>

extern uint16_t total_requests;
extern uint16_t ok_responses, error_responses, transaction_errors, ack_timeouts;


#endif /* RF_INFO_H_ */
