#ifndef __LABEL_H
#define __LABEL_H

#include <stdint.h>
#include "../windows.h"

typedef struct {
    window_t* parent;
    uint8_t* string;
    uint16_t x;
    uint16_t y;
    uint32_t color;
    uint16_t scale;
} label_t;

label_t create_label(window_t* parent, uint8_t* string, uint16_t x, uint16_t y, uint32_t color,
                    uint32_t scale) {
    return (label_t) { parent, string, x, y, color, scale };
}

void render_label(label_t label) {
    if (!label.parent->visible) return;
    window_set_cursor(label.parent, label.x, label.y);
    window_write_string(label.parent, label.string, label.color, label.scale);
}

#endif
