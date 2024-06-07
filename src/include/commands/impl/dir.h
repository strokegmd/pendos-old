//
// Created by kukuv on 06.06.2024.
//

#ifndef PENDOS_MAIN_1_DIR_H
#define PENDOS_MAIN_1_DIR_H

#include "../../tty.h"

void handle_dir(char *buffer) {
    fat_entry_t* entries = fat_list_files();
    tty_write_string("Files in /\n");
    for (uint8_t i = 0; i < 16; i++) {
        if (strlen(entries[i].name) < 1) continue;
        tty_write_string(entries[i].name);
        tty_putchar(' ');
        tty_write_string(itoa(entries[i].file_size));
        tty_write_string(" bytes ");
        tty_write_string("\n");
    }
    tty_putchar('\n');
}

#endif //PENDOS_MAIN_1_DIR_H
