#ifndef __RANDOM_H
#define __RANDOM_H

#include <stdint.h>
#include "../devices/pit.h"

/*
Generates random number in range
*/
uint32_t random_generate(uint32_t min, uint32_t max) {
    uint32_t seed = timer_ticks;
    seed = seed * 1103515245 + 12345;
    return min + ((seed / 65536) % max);
}

#endif
