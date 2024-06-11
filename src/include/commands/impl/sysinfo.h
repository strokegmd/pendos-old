#ifndef __SYSINFO_H
#define __SYSINFO_H

#include <stdint.h>

uint32_t ram_count = 0;

uint8_t* get_name(void) {
    fat_entry_t* entries = fat_list_files();
    for (uint16_t i = 0; i < 16; i++) {
        if (strncmp(entries[i].name, "NAME       ", 11)) {
            return fat_read_file(entries[i]);
        }
    }
}

uint8_t* get_pc(void) {
    fat_entry_t* entries = fat_list_files();
    for (uint16_t i = 0; i < 16; i++) {
        if (strncmp(entries[i].name, "PC         ", 11)) {
            return fat_read_file(entries[i]);
        }
    }
}

// why the hell im even using KDE Plasma logo
uint8_t* logo[11] = {
    "    %%%      %%%        ",
    "    %%%     %%%%%%      ",
    "             %%%%%%%    ",
    "               %%%%%%   ",
    "%%%%         %%%%%%%    ",
    "%%%%        %%%%%%      ",
    " %%          %%%        ",
    "                        ",
    "     %%%%%              ",
    "     %%%%%%             ",
    "     %%%%%              ",
};

void print_param(uint8_t* name, uint8_t* content) {
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_BLUE, TTY_COLOR_BLACK));
    tty_write_string(name);
    tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
    tty_write_string(": ");
    tty_write_string(content);
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_BLUE, TTY_COLOR_BLACK));
}

void handle_sysinfo() {
    for (uint8_t i = 0; i < 11; i++) {
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_BLUE, TTY_COLOR_BLACK));
        tty_write_string(logo[i]);
        switch (i) {
            case 0:
                tty_write_string(get_name());
                tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
                tty_putchar('@');
                tty_set_color(tty_get_color(TTY_COLOR_LIGHT_BLUE, TTY_COLOR_BLACK));
                tty_write_string(get_pc());
                break;
            case 1:
                for (uint16_t i = 0; i < strlen(get_name()) + strlen(get_pc()) + 1; i++) {
                    tty_putchar('-');
                }
                break;
            case 2:
                print_param("OS", "pendOS v1.0.00 Developer Preview 1");
                break;
            case 3:
                print_param("CPU Vendor", cpu_get_vendor());
                break;
            case 4:
                print_param("CPU Name", cpu_get_name());
                break;
            case 5:
                print_param("RAM Amount", itoa((ram_count / 1024 / 1024) + 1));
                tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
                tty_write_string("mb");
                tty_set_color(tty_get_color(TTY_COLOR_LIGHT_BLUE, TTY_COLOR_BLACK));
                break;
            case 6:
                print_param("Compiled At", __DATE__);
                tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
                tty_putchar(' ');
                tty_write_string(__TIME__);
            default:
                break;
        }
        tty_putchar('\n');
    }
    tty_putchar('\n');
}

#endif
