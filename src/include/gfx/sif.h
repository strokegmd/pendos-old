#ifndef __SIF_H
#define __SIF_H

#include <stdint.h>
#include <stdbool.h>

void draw_sif(uint8_t* data, uint32_t yOffset) {
    uint32_t index = 0;
    for (uint16_t y = 0; y < 300; y++) {
        for (uint16_t x = 0; x < 800; x++) {
            uint32_t color = data[index] * 65536 + data[index + 1] * 256 + data[index + 2];
            vbe_putpixel(x, yOffset + y, color);
            index += 3;
        }
    }
}

#endif
