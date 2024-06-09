#ifndef __SETPCNAME_H
#define __SETPCNAME_H

#include <stdint.h>
#include "../../fs/fat.h"
#include "../../lib/string.h"

void handle_setpcname(char* buffer) {
    uint8_t* pcname = strrem(buffer, 10);
    fat_write_file(fat_find_file_ptr("PC         "), pcname, strlen(pcname));
    tty_putchar('\n');
}

#endif
