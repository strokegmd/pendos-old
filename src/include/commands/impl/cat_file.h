//
// Created by kukuv on 06.06.2024.
//

#ifndef PENDOS_MAIN_1_CAT_FILE_H
#define PENDOS_MAIN_1_CAT_FILE_H

#include <stdint.h>

#include "../../fs/fat.h"
#include "../../lib/string.h"
#include "../../drivers/tty.h"
#include "../command.h"

void handle_cat(char *buffer) {
    fat_entry_t* entries = fat_list_files();
    for (uint8_t i = 0; i < 16; i++) {
        if (strlen(entries[i].name) < 1) continue;
        if (strncmp(strrem(buffer, 4), entries[i].name, 11)) {
            tty_write_string(fat_read_file(entries[i]));
        }
    }
    tty_write_string("\n\n");
}

#endif //PENDOS_MAIN_1_CAT_FILE_H
