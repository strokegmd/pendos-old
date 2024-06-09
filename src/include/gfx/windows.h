#ifndef __WINDOWS_H
#define __WINDOWS_H

#include "../drivers/vbe.h"
#include <stdint.h>

typedef struct {
    uint16_t x;
    uint16_t y;
} window_cursor_t;

typedef struct {
    uint8_t* title;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t visible;
    uint16_t show_titlebar;
    window_cursor_t* cursor;
} window_t;

/*
Creates window object
*/
window_t create_window(uint8_t* title, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    window_cursor_t* cursor;
    cursor->x = 0;
    cursor->y = 0;
    
    return (window_t) { title, x, y, width, height, true, true, cursor };
}

/*
Displays titlebar for window object
*/
void display_titlebar(window_t window) {
    if (!window.show_titlebar) return;
    vbe_rect(window.x, window.y - 20, window.width, 20, 0x222222);
    vbe_write_string(window.title, window.x + 10, window.y - 18, 0xffffff, 1);
}

/*
Plots a pixel on window
*/
void window_putpixel(window_t window, uint16_t x, uint16_t y, uint32_t color) {
    if (x > window.width) return;
    if (y > window.height) return;
    vbe_putpixel(window.x + x, window.y + y, color);
}

/*
Plots a rectangle on window
*/
void window_rect(window_t window, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                uint32_t color) {
    if (x > window.width) return;
    if (y > window.height) return;
    vbe_rect(window.x + x, window.y + y, width, height, color);
}

/*
Fills a window with a color
*/
void window_fill(window_t window, uint32_t color) {
    for (uint16_t i = 0; i < window.width; i++) {
        for (uint16_t j = 0; j < window.height; j++) {
            window_putpixel(window, i, j, color);
        }
    }
}

/*
Plots a character on window
*/
void window_putchar(window_t* window, uint8_t c, uint16_t color, uint16_t scale) {
    if (window->cursor->x > window->width) return;
    if (window->cursor->y > window->height) return;
    if (c == '\n') {
        window->cursor->y += 16 * scale;
        window->cursor->x = 0;
        return;
    }
    vbe_putchar(c, window->x + window->cursor->x, window->y + window->cursor->y, color, scale);
    window->cursor->x += 8 * scale;
}

/*
Displays window object
*/
void display_window(window_t window) {
    if (!window.visible) return;
    vbe_rect(window.x, window.y, window.width, window.height, 0xffffff);

    if (!window.show_titlebar) return;
    display_titlebar(window);
}

#endif
