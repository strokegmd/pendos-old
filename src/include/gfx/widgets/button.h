#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>
#include "../windows.h"
#include "label.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t* content;
    uint32_t x_pos;
    uint32_t y_pos;
    window_t* parent;
} button_t;

button_t create_button(window_t* parent, uint32_t width, uint32_t height, uint8_t* content, 
    uint32_t x, uint32_t y) {
    button_t btn = { width, height, content, x, y, parent };
    return btn;
}

void render_button(button_t button) {
    if (!button.parent->visible) return;

    window_t* win = button.parent;

    uint16_t pos_x = button.x_pos + button.width / 2 - strlen(button.content) * 4;
    uint16_t pos_y = button.y_pos + button.height / 2 - 8;

    window_rect(win, button.x_pos, button.y_pos, button.width + 2, button.height + 2, 0x8d8d8d);
    window_rect(win, button.x_pos, button.y_pos, button.width, button.height, 0xD3D3D3);
    render_label(create_label(win, button.content, pos_x, pos_y, 0, 1));
}

#endif
