#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "../system.h"

typedef struct {
    uint32_t present;
    void *addr;
    uint32_t size;
    uint32_t used;
} mem_block;

mem_block blocks[1024];
uint8_t *heap_ptr;

void *add_block(void *addr, int size, int used) {
    if (!size) return NULL;

    uint32_t i; for (i = 0; blocks[i].present; i++) ;
    blocks[i].present = true;
    blocks[i].addr = addr;
    blocks[i].size = size;
    blocks[i].used = used;
    return addr;
}

void *malloc(int size) {
    if (blocks[1023].present) {
        return NULL;
    }

    for (int i = 0; blocks[i].present; i++) {
        if (blocks[i].used && blocks[i].size >= size) {
            add_block(blocks[i].addr + size, blocks[i].size - size, true);
            blocks[i].size = size;
            blocks[i].used = false;
            return blocks[i].addr;
        }
    }

    void *addr = add_block(heap_ptr, size, false);
    heap_ptr += size;
    return addr;
}

void free(void *ptr) {
    for (int i = 0; blocks[i].present; i++) {
        if (blocks[i].used == false && blocks[i].addr == ptr) {
            blocks[i].used = true;
        }
    }
}

void *realloc(void *memblock, int size) {
    if(memblock < (void*)0x500) {
        return malloc(size);
    }
    else {
        void *newmem = malloc(size);
        if(newmem) {
            memmove(newmem, memblock, size);
            free(memblock);
        }
        return newmem;
    }
}

size_t init_memory() {
    heap_ptr = (uint8_t*) 0x1000000;
    memset(blocks, 0, sizeof(mem_block) * 1024);
}

#endif