#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>
#include "../grub/multiboot.h"
#include "../gfx/font.h"
#include "../lib/string.h"

multiboot_info_t* vbe_mbinfo;
void* framebuffer;

uint32_t backbuffer[800*600];
uint32_t pitch;
uint32_t size;

/*
Inits VBE mode
*/
void vbe_init(multiboot_info_t* mbinfo) {
    vbe_mbinfo = mbinfo;
    framebuffer = (void *) (unsigned long) vbe_mbinfo->framebuffer_addr;
    pitch = vbe_mbinfo->framebuffer_pitch / 4;
    size = vbe_mbinfo->framebuffer_pitch * vbe_mbinfo->framebuffer_height;
}

/*
Swaps buffers in VBE mode
*/
void vbe_swap_buffers() {
    memcpy(framebuffer, backbuffer, 800*600*4);
}

/*
Plots pixel on screen in VBE mode
*/
void vbe_putpixel(uint16_t x, uint16_t y, uint32_t color) {
    backbuffer[y * pitch + x] = color;
}

/*
Fills screen with a color in VBE mode
*/
void vbe_fill(uint32_t color) {
    memset((uint8_t*) backbuffer, color, size);
}

/*
Plots a rectangle on screen in VBE mode
TODO: optimize
*/
void vbe_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = x; i < x + width; i++) {
        for (uint16_t j = y; j < y + height; j++) {
            vbe_putpixel(i, j, color);
        }
    }
}

/*
Plots a character on screen in VBE mode
*/
void vbe_putchar(uint8_t character, uint32_t x, uint32_t y, uint32_t color, uint32_t scale) {
    if(character == '\n') return;

    uint16_t mask[8] = { 1, 2, 4 ,8, 16, 32, 64, 128 };
    uint8_t *glyph = vbe_font + (int) character * 16;
 
    for (uint32_t cy = 0; cy < 16; cy++){
        for (uint32_t cx = 0; cx < 8; cx++){
            if (glyph[cy] & mask[7-cx]) {
                for (uint32_t s = 0; s < scale; s++) {
                    for (uint32_t s1 = 0; s1 < scale; s1++) {
                        vbe_putpixel(x+cx*scale+s, y+cy*scale+s1, color);
                    }
                }
            }
        }
    }
}

/*
Plots a string on screen in VBE mode
*/
void vbe_write_string(uint8_t* str, uint32_t x, uint32_t y, uint32_t color, uint32_t scale) {
    for (uint16_t i = 0; i < strlen(str); i++) {
        vbe_putchar(str[i], x + i * 8 * scale, y, color, scale);
    }
}

#endif
