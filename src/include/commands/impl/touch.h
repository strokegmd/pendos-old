#ifndef __TOUCH_H
#define __TOUCH_H

#include "../../fs/fat.h"
#include "../../lib/string.h"

void handle_touch(char* buffer) {
    uint8_t* name = strrem(buffer, 6);
    fat_create_file(name);
    tty_putchar('\n');
}

#endif
