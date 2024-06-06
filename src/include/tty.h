#ifndef __TTY_H
#define __TTY_H

#include <stdint.h>
#include "string.h"

uint8_t* vidmem = (uint8_t*) 0xb8000;

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t screensize;
    uint8_t* video;
} tty_screen_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t color;
} tty_cursor_t;

typedef enum {
	TTY_COLOR_BLACK = 0,
	TTY_COLOR_BLUE = 1,
	TTY_COLOR_GREEN = 2,
	TTY_COLOR_CYAN = 3,
	TTY_COLOR_RED = 4,
	TTY_COLOR_MAGENTA = 5,
	TTY_COLOR_BROWN = 6,
	TTY_COLOR_LIGHT_GREY = 7,
	TTY_COLOR_DARK_GREY = 8,
	TTY_COLOR_LIGHT_BLUE = 9,
	TTY_COLOR_LIGHT_GREEN = 10,
	TTY_COLOR_LIGHT_CYAN = 11,
	TTY_COLOR_LIGHT_RED = 12,
	TTY_COLOR_LIGHT_MAGENTA = 13,
	TTY_COLOR_LIGHT_BROWN = 14,
	TTY_COLOR_WHITE = 15,
} tty_color_t;

tty_screen_t screen = { 80, 25, 80 * 25, (uint8_t*) 0xb8000 };
tty_cursor_t cursor = { 0, 0, TTY_COLOR_LIGHT_GREY };

/*
Gets color in TTY mode
*/
tty_color_t tty_get_color(tty_color_t fg, tty_color_t bg) {
    return fg | bg << 4;
}

/*
Sets cursor color in TTY mode.
*/
void tty_set_color(uint16_t color) {
    cursor.color = color;
}

/*
Clears screen in TTY mode
*/
void tty_clear() {
    for (size_t i = 0; i < screen.width * screen.height * 2; i += 2) {
        screen.video[i] = ' ';
        screen.video[i + 1] = cursor.color;
    }
}

/*
Prints character
*/
void tty_putchar(uint8_t character) {
    uint16_t pos = (cursor.x + cursor.y * screen.width) * 2;

    switch (character) {
        case '\n':
            cursor.x = 0;
            cursor.y++;
            break;
        default:
            screen.video[pos] = character;
            screen.video[pos + 1] = cursor.color;
            cursor.x++;
            break;
    }
}

/*
Prints string
*/
void tty_write_string(uint8_t* str) {
    for (size_t i = 0; i < strlen(str); i++) {
        tty_putchar(str[i]);
    }
}

#endif