#ifndef __INIT_H
#define __INIT_H

// systemd: finally a great opponent
// welcum (hohohaha cum in 2024) to pendos-init

#include <stdint.h>
#include "drivers/tty.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/irq.h"
#include "lib/memory.h"
#include "devices/pit.h"

bool skip_wait = true; // lmao you can run pindosik without wait kruto da?????? a vash linuks tak ymeet???

void print_welcome() {
    tty_write_string("\nWelcome to ");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_CYAN, TTY_COLOR_BLACK));
    tty_write_string("pendOS");
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string("!\n\n");
}

void init_log_name(uint8_t* component) {
    tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
    tty_putchar('[');
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREEN, TTY_COLOR_BLACK));
    tty_write_string("INIT");
    tty_set_color(tty_get_color(TTY_COLOR_WHITE, TTY_COLOR_BLACK));
    tty_write_string("] ");

    tty_write_string(component);
    
    for (uint8_t i = 0; i < 3; i++) {
        if (skip_wait) continue;
        tty_putchar('.');
        pit_sleep(25);
    }

    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK));
    tty_write_string(" ok.\n");
}

void init_log(uint8_t* component, void (*init_function)()) {
    init_log_name(component);
    init_function();
}

void init(multiboot_info_t* mbinfo) {
    print_welcome();

    gdt_install();
    idt_install();
    interrupt_disable_all();
    pit_install();
    keyboard_install();
    interrupt_enable_all();

    init_log_name("Global Descriptor Table");
    init_log_name("Interrupt Descriptor Table");
    init_log_name("Interrupts");
    init_log_name("Programmable Interval Timer");
    init_log_name("Memory Manager"); init_memory();
    init_log_name("PS/2 Keyboard Driver");
    init_log("PS/2 Mouse Driver", mouse_install);
    init_log_name("VBE Graphics Mode"); vbe_init(mbinfo);
    init_log("Desktop", desktop_init);

    tty_set_cursor(0, 0);
}

#endif
