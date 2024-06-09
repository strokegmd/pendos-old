//
// Created by kukuv on 06.06.2024.
//

#ifndef PENDOS_MAIN_1_SHUTDOWN_H
#define PENDOS_MAIN_1_SHUTDOWN_H

#include "../../system.h"
#include "../../drivers/tty.h"

void handle_shutdown() {
    shutdown();
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
    tty_write_string("ERROR: ");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("Shutdown currently only working on Virtual Machines and Emulators.\n\n");
}

#endif //PENDOS_MAIN_1_SHUTDOWN_H
