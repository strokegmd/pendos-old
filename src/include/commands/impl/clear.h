//
// Created by kukuv on 06.06.2024.
//

#ifndef PENDOS_MAIN_1_CLEAR_H
#define PENDOS_MAIN_1_CLEAR_H

#include "../../tty.h"

void handle_clear() {
    tty_clear();
    cursor.x = 0;
    cursor.y = 0;
    tty_write_string("Welcome to ");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK));
    tty_write_string("pendOS");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("!\n\n");
}

#endif //PENDOS_MAIN_1_CLEAR_H
