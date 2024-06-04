#include "include/tty.h"
#include "include/system.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/irq.h"
#include "include/pit.h"

void kmain(void) {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    pit_install();
    interrupt_enable_all();

    pit_sleep(1000);
    
    tty_set_color(tty_get_color(TTY_COLOR_CYAN, TTY_COLOR_RED));
    tty_clear();
    tty_putchar('z');
    tty_putchar('v');
}