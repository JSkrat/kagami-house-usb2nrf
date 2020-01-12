#include "tests.h"
#include <stdint.h>

void *pgm_read_ptr_near(void *ptr) {
    return *(reinterpret_cast<void **>(ptr));
}

void *pgm_read_ptr(void *ptr) {
    return *(reinterpret_cast<void **>(ptr));
}

uint8_t pgm_read_byte(uint8_t *ptr) {
    return *ptr;
}
