#include "include/grub/multiboot.h"
#include "include/drivers/vbe.h"
#include "kernel_tty.h"
#include "kernel_vbe.h"

#define TEXT_MODE_WIDTH 80

void kmain(multiboot_info_t* mbinfo) {
    vbe_init(mbinfo);
    if (vbe_mbinfo->framebuffer_width == TEXT_MODE_WIDTH) {
        kmain_tty(mbinfo);
        return;
    }

    kmain_vbe(mbinfo);
}