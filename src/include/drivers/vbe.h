#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>
#include <stdbool.h>
#include "../grub/multiboot.h"
#include "../lib/string.h"
#include "../lib/math.h"
#include "../fs/fat.h"

multiboot_info_t* vbe_mbinfo;
void* framebuffer;

uint32_t backbuffer[800*600];
uint32_t pitch;
uint32_t size;
uint8_t* font;

/*
Inits VBE mode
*/
void vbe_init(multiboot_info_t* mbinfo) {
    vbe_mbinfo = mbinfo;
    framebuffer = (void *) (unsigned long) vbe_mbinfo->framebuffer_addr;
    pitch = vbe_mbinfo->framebuffer_pitch / 4;
    size = vbe_mbinfo->framebuffer_pitch * vbe_mbinfo->framebuffer_height;
    font = fat_read_file(fat_find_file("DEFAULT FNT"));
}

/*
Swaps buffers in VBE mode
*/
void vbe_swap_buffers() {
    memmove(framebuffer, backbuffer, 800*600*4);
}

/*
Plots pixel on screen in VBE mode
*/
void vbe_putpixel(uint16_t x, uint16_t y, uint32_t color) {
    backbuffer[y * pitch + x] = color;
}

/*
Plots pixel with transparency on screen in VBE mode
TODO: please do not use, need to optimize suka
*/
void vbe_putpixel_blend(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    uint32_t* pixel_addr = &backbuffer[y * pitch + x];
    uint32_t background_color = *pixel_addr;
    uint8_t br = (background_color >> 16) & 0xFF;
    uint8_t bg = (background_color >> 8) & 0xFF;
    uint8_t bb = background_color & 0xFF;

    uint8_t ar = (a * r + (~a & 0xFF) * br) >> 8;
    uint8_t ag = (a * g + (~a & 0xFF) * bg) >> 8;
    uint8_t ab = (a * b + (~a & 0xFF) * bb) >> 8;

    *pixel_addr = (ar << 16) | (ag << 8) | ab;
}

/*
Fills screen with a color in VBE mode
*/
void vbe_fill(uint32_t color) {
    for (int i = 0; i < size / 4; i++) {
        ((uint32_t*)backbuffer)[i] = color;
    }
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
    if (character == '\n') return;

    uint16_t mask[8] = { 1, 2, 4 ,8, 16, 32, 64, 128 };
    uint8_t *glyph = font + (uint16_t) character * 16;
 
    for (uint32_t cy = 0; cy < 16; cy++) {
        for (uint32_t cx = 0; cx < 8; cx++) {
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
        if (str[i] == '\n') {
            y += 8 * scale;
        }
        
        vbe_putchar(str[i], x + i * 8 * scale, y, color, scale);
    }
}

/*
Draws a rounded rectangle on screen
*/
void vbe_rounded_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, 
                      uint32_t radius, uint32_t border_color,
                      bool top, bool bottom) {
    uint32_t i, j;

    for (i = x + radius; i < x + width - radius; i++) {
        vbe_putpixel(i, y, border_color);
        vbe_putpixel(i, y + height - 1, border_color);
    }

    for (j = y + radius; j < y + height - radius; j++) {
        vbe_putpixel(x, j, border_color);
        vbe_putpixel(x + width - 1, j, border_color);
    }

    if (top) {
        for (i = x; i < x + radius; i++) {
            for (j = y; j < y + radius; j++) {
                if ((i - x - radius) * (i - x - radius) + (j - y - radius) * (j - y - radius) <= radius * radius) {
                    vbe_putpixel(i, j, border_color);
                }
            }
        }

        for (i = x + width - radius; i < x + width; i++) {
            for (j = y; j < y + radius; j++) {
                if ((i - x - width + radius) * (i - x - width + radius) + (j - y - radius) * (j - y - radius) <= radius * radius) {
                    vbe_putpixel(i, j, border_color);
                }
            }
        }
    }
    else {
        vbe_rect(x, y, width, radius, border_color);
    }

    if (bottom) {
        for (i = x; i < x + radius; i++) {
            for (j = y + height - radius; j < y + height; j++) {
                if ((i - x - radius) * (i - x - radius) + (j - y - height + radius) * (j - y - height + radius) <= radius * radius) {
                    vbe_putpixel(i, j, border_color);
                }
            }
        }

        for (i = x + width - radius; i < x + width; i++) {
            for (j = y + height - radius; j < y + height; j++) {
                if ((i - x - width + radius) * (i - x - width + radius) + (j - y - height + radius) * (j - y - height + radius) <= radius * radius) {
                    vbe_putpixel(i, j, border_color);
                }
            }
        }
    }
    else {
        vbe_rect(x, y+height-radius, width, radius, border_color);
    }

    vbe_rect(x, y + radius, width, height - (radius * 2), border_color);
    vbe_rect(x + radius, y, width - (radius * 2), height, border_color);
}

#endif
