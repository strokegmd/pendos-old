#include "include/tty.h"
#include "include/system.h"
#include "include/cpu/gdt.h"
#include "include/cpu/idt.h"
#include "include/cpu/irq.h"
#include "include/devices/pit.h"
#include "include/random.h"
#include "include/drivers/ps2/keyboard.h"
#include "include/memory.h"
#include "include/fs/fat.h"
#include "include/rtc.h"

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

void prompt(void) {
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
    tty_write_string("root@");

    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("pendos ~ # ");

    uint8_t* buffer = get_buffer();
    if (strcmp(buffer, "help")) {
        tty_write_string("help mess\n\n");
    } else if (strcmp(buffer, "ls") || strcmp(buffer, "dir")) {
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
    } else if (strcmp(buffer, "reboot")) {
        reboot();
    } else if (strcmp(buffer, "clear") || strcmp(buffer, "cls")) {
        tty_clear();
        cursor.x = 0;
        cursor.y = 0;
        tty_write_string("Welcome to ");
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK));
        tty_write_string("pendOS");
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
        tty_write_string("!\n\n");
    } else if (strcmp(buffer, "time")) {
        read_rtc();
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
        tty_write_string("Current time: ");
        tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
        if (hour < 10) tty_putchar('0'); tty_write_string(itoa(hour)); tty_putchar(':');
        if (minute < 10) tty_putchar('0'); tty_write_string(itoa(minute)); tty_putchar(':');
        if (second < 10) tty_putchar('0'); tty_write_string(itoa(second)); tty_putchar('\n');
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
        tty_write_string("Current date: ");
        tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
        if (day < 10) tty_putchar('0'); tty_write_string(itoa(day)); tty_putchar('.');
        if (month < 10) tty_putchar('0'); tty_write_string(itoa(month)); tty_putchar('.');
        if (year < 10) tty_putchar('0'); tty_write_string(itoa(year)); tty_write_string("\n\n");
    } else if (strcmp(buffer, "shutdown")) {
        shutdown();
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
        tty_write_string("ERROR: ");
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
        tty_write_string("Shutdown currently only working on Virtual Machines and Emulators.\n\n");
    } else if (strstartswith(buffer, "cat ", 4)) {
        fat_entry_t* entries = fat_list_files();
        for (uint8_t i = 0; i < 16; i++) {
            if (strlen(entries[i].name) < 1) continue;
            if (strncmp(strrem(buffer, 4), entries[i].name, 11)) {
                tty_write_string(fat_read_file(entries[i]));
            }
        }
        tty_write_string("\n\n");
    } else {
        tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
        tty_write_string("Unknown command!\n\n");
    }

    prompt();
}

void kmain(void) {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    pit_install();
    keyboard_install();
    interrupt_enable_all();
    
    tty_clear();

    tty_write_string("Welcome to ");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK));
    tty_write_string("pendOS");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("!\n\n");

    prompt();
}
