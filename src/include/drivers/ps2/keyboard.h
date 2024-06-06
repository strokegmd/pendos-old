#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#include <stdint.h>
#include <stdbool.h>

bool shift = false;

uint8_t kb_key = 0;

const uint8_t keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, 
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const uint8_t keymap_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, 
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

volatile uint8_t key_buffer[128];
volatile uint8_t buffer_head = 0;
volatile uint8_t buffer_tail = 0;

/*
Keyboard handler
*/
void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        kb_key = 0;
        shift = scancode == 0xAA ? false : shift;
    } else {
        shift = scancode == 0x2A ? true : shift;
        uint8_t c = shift ? keymap_shift[scancode] : keymap[scancode];
        if (c) {
            key_buffer[buffer_head] = c;
            buffer_head = (buffer_head + 1) % 128;
        }
    }
}

/*
Gets key from keyboard
*/
uint8_t keyboard_get_key() {
    if (buffer_head == buffer_tail) {
        return 0;
    } else {
        uint8_t c = key_buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % 128;
        return c;
    }
}

/*
Installs keyboard
*/
void keyboard_install() {
    register_interrupt_handler(1, keyboard_handler);
}

#endif
