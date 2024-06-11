#ifndef __CHECKBOX_H
#define __CHECKBOX_H

#include <stdint.h>
#include <stdbool.h>
#include "../windows.h"

typedef struct {
    bool value;
    uint32_t x_pos;
    uint32_t y_pos;
    window_t *parent;
} checkbox_t;

checkbox_t create_checkbox(window_t* parent, bool value, uint32_t x, uint32_t y) {
    return (checkbox_t) { value, x, y, parent };
}

void render_check_mark(window_t* window, uint16_t x, uint16_t y, uint16_t size) {
    for (uint16_t i = 0; i < size / 2; i++) {
        window_putpixel(window, x - i, y + size - i - 1, 0);
    }

    for (uint16_t i = 0; i < size; i++) {
        window_putpixel(window, x + i, y + size - i - 1, 0);
    }
}

void render_checkbox(checkbox_t checkbox) {
    window_rect(checkbox.parent, checkbox.x_pos, checkbox.y_pos, 18, 18, 0x8d8d8d);
    window_rect(checkbox.parent, checkbox.x_pos, checkbox.y_pos, 16, 16, 0xD3D3D3);

    if (!checkbox.value) return;
    render_check_mark(checkbox.parent, checkbox.x_pos + 5, checkbox.y_pos + 3, 10);
}

#endif
