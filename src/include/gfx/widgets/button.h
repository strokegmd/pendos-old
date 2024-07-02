#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>
#include "../windows.h"
#include "../../drivers/ps2/mouse.h"
#include "../rgb.h"
#include "label.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t* content;
    uint32_t x_pos;
    uint32_t y_pos;
    window_t* parent;

    void (*onButtonClicked)();
} button_t;

button_t create_button(window_t* parent, uint32_t width, uint32_t height, uint8_t* content, 
    uint32_t x, uint32_t y, void (*onButtonClicked)()) {
    return (button_t) {
            width, height, content, x, y, parent, onButtonClicked
    };
}

void render_button(button_t button) {
    if (!button.parent->visible) return;

    uint32_t x_pos = button.x_pos + button.parent->x;
    uint32_t y_pos = button.y_pos + button.parent->y;

    bool isHovered = mouse_x > x_pos && mouse_x < x_pos + button.width
                  && mouse_y > y_pos && mouse_y < y_pos + button.height;

    window_t* win = button.parent;

    uint16_t pos_x = button.x_pos + button.width / 2 - strlen(button.content) * 4;
    uint16_t pos_y = button.y_pos + button.height / 2 - 8;

    pixel_t col1 = from_hex(0x8d8d8d);
    pixel_t col2 = from_hex(0xD3D3D3);

    if (isHovered) {
        col1 = darker(col1);
        col2 = darker(col2);

        if (mouse_button == 1) {
            col1 = darker(col1);
            col2 = darker(col2);
        }
        
        if (mouse_button_prev == 1 && mouse_button == 0) {
            button.onButtonClicked();
        }
    }

    window_rect(win, button.x_pos, button.y_pos, button.width + 2, button.height + 2, from_rgb(col1));
    window_rect(win, button.x_pos, button.y_pos, button.width, button.height, from_rgb(col2));
    render_label(create_label(win, button.content, pos_x, pos_y, 0, 1));
}

#endif
