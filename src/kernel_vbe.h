#ifndef __KERNEL_VBE_H
#define __KERNEL_VBE_H

#include "include/drivers/vbe.h"
#include "include/grub/multiboot.h"
#include "include/cpu/gdt.h"
#include "include/cpu/idt.h"
#include "include/cpu/irq.h"
#include "include/fs/fat.h"
#include "include/gfx/windows.h"
#include "include/devices/pit.h"
#include "include/drivers/ps2/mouse.h"
#include "include/drivers/ps2/keyboard.h"

void kmain_vbe(multiboot_info_t* mbinfo) {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    keyboard_install();
    pit_install();
    mouse_install();
    interrupt_enable_all();
    vbe_init(mbinfo);
    vbe_fill(0x323232);

    uint32_t x = 100;
    uint32_t y = 100;

    for(;;) {
        uint32_t start = timer_ticks;
        vbe_fill(0x323232);
        window_t win = create_window("zvohui", x,y, 200, 300);
        display_window(win);
        x+=1;
        vbe_putpixel(mouse_x, mouse_y, 0xffffff);
        uint32_t end = timer_ticks;
        vbe_write_string(itoa(end-start), 20,20, 0xffffff, 1);
        vbe_write_string(itoa(mouse_button), 50,20, 0xffffff, 1);
        vbe_swap_buffers();
    }
}

#endif
