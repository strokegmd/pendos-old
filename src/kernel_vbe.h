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
#include "include/gfx/widgets/label.h"
#include "include/gfx/widgets/button.h"
#include "include/gfx/widgets/checkbox.h"
#include "include/gfx/sif.h"

void kmain_vbe(multiboot_info_t* mbinfo) {
    gdt_install();
    idt_install();
    interrupt_disable_all();
    keyboard_install();
    pit_install();
    mouse_install();
    interrupt_enable_all();
    vbe_init(mbinfo);

    for(;;) {
        draw_sif(fat_read_file(fat_find_file("BACKGRO1SIF")), 0);
        draw_sif(fat_read_file(fat_find_file("BACKGRO2SIF")), 298);
        vbe_swap_buffers();
    }
}

#endif
