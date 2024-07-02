#ifndef __DESKTOP_H
#define __DESKTOP_H

#include <stdint.h>
#include "../fs/fat.h"
#include "../drivers/ps2/mouse.h"
#include "../drivers/vbe.h"
#include "../temp/background.h"
#include "sif.h"

uint8_t* cursor_data;

void desktop_init() {
    cursor_data = fat_read_file(fat_find_file("ADWAITA SIF"));
}

void draw_wallpaper() {
    draw_sif(wallpaper, 0, 0, 800, 600);
}

void draw_cursor() {
    draw_sif(cursor_data, mouse_x, mouse_y, 24, 24);
}

void draw_taskbar_time() {
    uint16_t time_x = 720;
    uint16_t time_y = 27;
    uint16_t offset = 16;

    if (hour < 10) vbe_putchar('0', time_x, time_y, 0xffffff, 1);
    else offset += 8;
    vbe_write_string(itoa(hour), time_x + 8, time_y, 0xffffff, 1);
    vbe_putchar(':', time_x + offset, time_y, 0xffffff, 1);

    if (minute < 10) vbe_putchar('0', time_x + offset + 8, time_y, 0xffffff, 1);
    else offset += 8;
    vbe_write_string(itoa(minute), time_x + offset + 16, time_y, 0xffffff, 1);
}

void draw_taskbar() {
    vbe_rounded_rect(15, 15, 770, 40, 20, 0x222222, true, true);
    draw_taskbar_time();
}

#endif
