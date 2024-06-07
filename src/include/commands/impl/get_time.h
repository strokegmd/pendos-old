//
// Created by kukuv on 06.06.2024.
//

#ifndef PENDOS_MAIN_1_GET_TIME_H
#define PENDOS_MAIN_1_GET_TIME_H

#include "../../tty.h"
#include "../../rtc.h"

void handle_get_time() {
    read_rtc();
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("Current time: ");
    tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
    if (hour < 10) tty_putchar('0'); tty_write_string(itoa(hour)); tty_putchar(':');
    if (minute < 10) tty_putchar('0'); tty_write_string(itoa(minute)); tty_putchar(':');
    if (second < 10) tty_putchar('0'); tty_write_string(itoa(second)); tty_putchar('\n');
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("Current date: ");
    tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
    if (day < 10) tty_putchar('0'); tty_write_string(itoa(day)); tty_putchar('.');
    if (month < 10) tty_putchar('0'); tty_write_string(itoa(month)); tty_putchar('.');
    if (year < 10) tty_putchar('0'); tty_write_string(itoa(year)); tty_write_string("\n\n");
}

#endif //PENDOS_MAIN_1_GET_TIME_H
