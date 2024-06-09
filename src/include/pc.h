#ifndef __PC_H
#define __PC_H

#include "grub/multiboot.h"

/*
Counts memory using GRUB Memory Map
*/
uint32_t ram_get_bytes(multiboot_info_t* mbinfo) {
    uint32_t value = 0;
    for (uint32_t i = 0; i < mbinfo->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbinfo->mmap_addr + i);
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            value += mmmt->len_low;
        }
    }
    return value;
}

void cpu_id(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    __asm__ volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

uint8_t* cpu_get_vendor() {
    uint32_t largestStandardFunc;
    uint8_t* vendor = "@@@@@@@@@@@@@"; // 13 symbols for vendor
    cpu_id(0, &largestStandardFunc, (uint32_t*)(vendor + 0), (uint32_t*)(vendor + 8), 
         (uint32_t*)(vendor + 4));

    vendor[12] = '\0';
    return vendor;
}

uint8_t* cpu_get_name() {
    uint8_t* name = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"; // 48 symbols for name
    cpu_id(0x80000002, (uint32_t*)(name +  0), (uint32_t*)(name +  4), (uint32_t*)(name +  8), (uint32_t*)(name + 12));
    cpu_id(0x80000003, (uint32_t*)(name + 16), (uint32_t*)(name + 20), (uint32_t*)(name + 24), (uint32_t*)(name + 28));
    cpu_id(0x80000004, (uint32_t*)(name + 32), (uint32_t*)(name + 36), (uint32_t*)(name + 40), (uint32_t*)(name + 44));

    uint8_t* p = name;
    while (*p == ' ') {
        ++p;
    }

    return (uint8_t*) p;
}

#endif
