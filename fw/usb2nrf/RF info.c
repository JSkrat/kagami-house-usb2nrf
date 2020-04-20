/*
 * RF_info.c
 *
 * Created: 20.04.2020 21:38:18
 *  Author: Mintytail
 */ 
#include "RF info.h"
#include <stdint.h>


uint16_t total_requests = 0, ok_responses = 0, error_responses = 0, transaction_errors = 0, ack_timeouts = 0;
