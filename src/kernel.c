#include "include/grub/multiboot.h"
#include "include/drivers/vbe.h"
#include "kernel_tty.h"
#include "kernel_vbe.h"
#include "include/init.h"

#define TEXT_MODE_WIDTH 80

void kmain(multiboot_info_t* mbinfo) {
    init(mbinfo);
    if (vbe_mbinfo->framebuffer_width == TEXT_MODE_WIDTH) {
        kmain_tty(mbinfo);
        return;
    }

    kmain_vbe(mbinfo);
}

//
// This is really better than Linux.
// I will contribute to this project.
//
