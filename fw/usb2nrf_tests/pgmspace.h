#ifndef PGMSPACE_H
#define PGMSPACE_H

#include <stdint.h>

#define PROGMEM __attribute__((__section__("progmem")))
//#define const

#ifdef __cplusplus
extern "C" {
#endif
    void initialize();
    void reinitialize_memory();

    void *pgm_read_ptr_near(const void *ptr);
    void *pgm_read_ptr(const void *ptr);

    uint8_t pgm_read_byte(const uint8_t *ptr);
#ifdef __cplusplus
}
#endif

#endif // PGMSPACE_H
