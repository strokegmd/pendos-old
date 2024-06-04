#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>

#define UNUSED(x) (void)(x)

/*
Halts CPU
*/
void halt() {
    __asm__("hlt");
}

/*
Get byte from port
*/
uint8_t inb(uint16_t port) {
	uint8_t result;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

/*
Get word from port
*/
uint16_t inw(uint16_t port) {
	uint16_t result;
	__asm__ __volatile__ ("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

/*
Write byte to port
*/
void outb(uint16_t port, uint8_t data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

/*
Write word to port
*/
void outw(uint16_t port, uint16_t data) {
	__asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}

/*
Shutdowns device.
Work only on emulators and virtual machines.
*/
void shutdown() {
    outw(0xB004, 0x2000);
    outw(0x604, 0x2000);
    outw(0x4004, 0x3400);
    outw(0x600, 0x34);
}

/*
Reboots device
*/
void reboot() {
    __asm__("jmp 0");
}

/*
Sets memory
*/
void memset(void* src, uint8_t val, uint32_t len) {
    uint8_t* ptr = (uint8_t*)src;
    while (len--) *ptr++ = val;
}

/*
Copy to memory
*/
void* memcpy(void* dst, void* src, uint32_t len) {
    uint8_t *d = (uint8_t*)dst;
    const uint8_t *s = (const uint8_t*)src;
    while (len-- > 0) *d++ = *s++;
    return dst;
}

#endif
