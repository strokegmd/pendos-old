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

void kmain(void) {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    pit_install();
    keyboard_install();
    interrupt_enable_all();
    
    tty_clear();

    fat_entry_t* p = fat_list_files();
    for (int i =0; i < 16;i++) {
        if(strlen(p[i].name) < 1) continue;
        tty_write_string(p[i].name);
        tty_write_string("\n\n");
        tty_write_string(fat_read_file(p[i]));
        tty_write_string("\n\n");
    }
}