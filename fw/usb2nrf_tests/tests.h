#ifndef TESTS_H
#define TESTS_H

#include <stdint.h>

#define PROGMEM __attribute__((__section__("progmem")))
//#define const

#ifdef __cplusplus
extern "C" {
#endif
    void *pgm_read_ptr_near(void *ptr);
    void *pgm_read_ptr(void *ptr);

    uint8_t pgm_read_byte(uint8_t *ptr);
#ifdef __cplusplus
}
#endif

#endif // TESTS_H
