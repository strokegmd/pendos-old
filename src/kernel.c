#include "include/system.h"
#include "include/rtc.h"

#include "include/cpu/gdt.h"
#include "include/cpu/idt.h"
#include "include/cpu/irq.h"

#include "include/devices/pit.h"

#include "include/lib/random.h"
#include "include/lib/memory.h"
#include "include/fs/fat.h"

#include "include/commands/command.h"

#include "include/commands/impl/cat_file.h"
#include "include/commands/impl/clear.h"
#include "include/commands/impl/dir.h"
#include "include/commands/impl/get_time.h"
#include "include/commands/impl/reboot.h"
#include "include/commands/impl/shutdown.h"
#include "include/commands/impl/touch.h"
#include "include/commands/impl/write.h"
#include "include/commands/impl/setname.h"
#include "include/commands/impl/setpcname.h"
#include "include/commands/impl/echo.h"

#include "include/drivers/ps2/keyboard.h"

#include "include/tty.h"

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

uint8_t add_command(command_t command) {
    uint8_t state;
    push_command(&command, &state);
    return state;
}

void inputLoop() {
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
    tty_write_string(get_name());
    tty_putchar('@');

    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string(get_pc());
    tty_write_string(" ~ # ");

    uint8_t* buffer = get_buffer();
    handle_commands(buffer);

    inputLoop();
}

void init() {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    pit_install();
    keyboard_install();
    interrupt_enable_all();
}

void kmain(void) {
    init();
    
    tty_clear();

    tty_write_string("Welcome to ");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK));
    tty_write_string("pendOS");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("!\n\n");

    add_command((command_t) { "dir", handle_dir });
    add_command((command_t) { "ls", handle_dir });
    add_command((command_t) { "cat", handle_cat });
    add_command((command_t) { "clear", handle_clear });
    add_command((command_t) { "cls", handle_clear });
    add_command((command_t) { "time", handle_get_time });
    add_command((command_t) { "reboot", handle_reboot });
    add_command((command_t) { "shutdown", handle_shutdown });
    add_command((command_t) { "touch", handle_touch });
    add_command((command_t) { "write", handle_write });
    add_command((command_t) { "setname", handle_setname });
    add_command((command_t) { "setpcname", handle_setpcname });
    add_command((command_t) { "echo", handle_echo });

    inputLoop();
}
