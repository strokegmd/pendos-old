#ifndef __SETNAME_H
#define __SETNAME_H

#include <stdint.h>
#include "../../fs/fat.h"
#include "../../lib/string.h"

void handle_setname(char* buffer) {
    uint8_t* name = strrem(buffer, 8);
    fat_write_file(fat_find_file_ptr("NAME       "), name, strlen(name));
    tty_putchar('\n');
}

#endif
