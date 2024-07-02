#ifndef __KERNEL_VBE_H
#define __KERNEL_VBE_H

#include "include/drivers/vbe.h"
#include "include/grub/multiboot.h"
#include "include/fs/fat.h"
#include "include/gfx/windows.h"
#include "include/devices/pit.h"
#include "include/drivers/ps2/mouse.h"
#include "include/drivers/ps2/keyboard.h"
#include "include/gfx/widgets/label.h"
#include "include/gfx/widgets/button.h"
#include "include/gfx/widgets/checkbox.h"
#include "include/drivers/vbe.h"
#include "include/gfx/desktop.h"
#include "include/gfx/windows.h"
#include "include/gfx/widgets/label.h"

void kmain_vbe(multiboot_info_t* mbinfo) {
    window_t win = create_window("strokegmd", 300, 200, 256, 256);

    label_t label = create_label(&win, "ya dolbaeb x2", 0, 0, 0, 1);
    button_t btn = create_button(&win, 100, 50, "click here", 20, 20, reboot);

    for(;;) {
        read_rtc();
        uint32_t start = timer_ticks;
        draw_wallpaper();
        draw_cursor();
        draw_taskbar();
        uint32_t end = timer_ticks;
        vbe_write_string(itoa(end - start), 0, 500, 0xffffff, 1);
        vbe_swap_buffers();
    }
}

#endif
