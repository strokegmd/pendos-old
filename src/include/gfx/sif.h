#include <stdint.h>
#include "../drivers/vbe.h"

/**
 * Draws Stroke Image Format file.
 * For more information, check img2sif.py.
 * WARNING: (ahtung) will be deprecated after Multimedia Update.
 * 
 * pizdec ya anglichanin odnako....................
 */
void draw_sif(uint8_t* data, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) {
    uint32_t index = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t color = from_rgb((pixel_t) { data[index], data[index+1], data[index+2] });
            if (data[index+3] < 110) {
                index += 4;
                continue;
            }
            vbe_putpixel(x + xOffset, y + yOffset, color);
            index += 4;
        }
    }
}