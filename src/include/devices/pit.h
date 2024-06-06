#ifndef __PIT_H
#define __PIT_H

#include "../system.h"
#include "../cpu/irq.h"
#include <stdint.h>

#define PIT_FREQUENCY 1193182
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

#define PIT_COMMAND 0x36

uint32_t timer_ticks = 0;

/*
Sets PIT frequency
*/
void pit_set_frequency(uint32_t frequency) {
    uint16_t divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND_PORT, PIT_COMMAND);
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); 
}

/*
Sleeps for some time
Currently blocking all OS
*/
void pit_sleep(uint16_t ms) {
    uint32_t start = timer_ticks;
    while (timer_ticks - start < ms) {
        halt();
    }
}

/*
PIT handler
*/
void pit_handler(struct regs *r) {
    UNUSED(r);
    timer_ticks++;
    outb(0x20, 0x20);
}

/*
Installs PIT with frequency 1000
*/
void pit_install() {
    pit_set_frequency(1000);
    register_interrupt_handler(0, pit_handler);
}

#endif
