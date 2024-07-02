#include <stdint.h>

#include "../include/drivers/tty.h"
#include "../include/cpu/gdt.h"
#include "../include/cpi/idt.h"
#include "../include/cpu/irq.h"
#include "../include/drivers/ps2/keyboard.h"

uint8_t* get_buffer(void) {
    static uint8_t buffer[65535];
    memset(buffer, 0, 65535);

    uint16_t i = 0;
    uint16_t startX = cursor.x;

    while (true) {
        uint8_t key = keyboard_get_key();
        if (key) {
            if (key == '\n') {
                tty_putchar('\n');
                return buffer;
            } else if (key == '\b') {
                if (cursor.x > startX) {
                    i--;
                    buffer[i] = key;
                    tty_set_cursor(cursor.x-1, cursor.y);
                    tty_putchar(' ');
                    tty_set_cursor(cursor.x-1, cursor.y);
                    tty_update_cursor();
                } else {
                    tty_putchar(' ');
                    tty_set_cursor(cursor.x-1, cursor.y);
                    tty_update_cursor();
                }
            } else {
                buffer[i] = key;
                tty_putchar(key);
                i++;
            }
        }
    }
}

void main() {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    keyboard_install();
    interrupt_enable_all();
    tty_clear();
    tty_write_string("welcome to pendos very first program!!!\n\ninsert your name: ");
    uint8_t* data = get_buffer();
    tty_write_string("\nyour name: ");
    tty_write_string(data);
}