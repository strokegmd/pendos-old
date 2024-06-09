#ifndef __WRITE_H
#define __WRITE_H

void handle_write(char* buffer) {
    uint8_t* name = strrem(buffer, 6);
    uint8_t* content = strrem(buffer, 6+12);
    fat_write_file(fat_find_file_ptr(name), content, strlen(content));
    tty_putchar('\n');
}

#endif
