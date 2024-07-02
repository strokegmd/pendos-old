#ifndef __EXEC_H
#define __EXEC_H

#include "../../exec/elf.h"

void handle_exec(char* input) {
    uint8_t* filename = strrem(input, 5);
    uint8_t* data = fat_read_file(fat_find_file(filename));
    if (!elf_check_file(data)) {
        tty_write_string("Not ELF file!\n\n");
    } else {
        elf_start(data);
    }
}

#endif
