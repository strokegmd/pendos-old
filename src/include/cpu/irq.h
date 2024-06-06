#ifndef __IRQ_H
#define __IRQ_H

#define IDT_NUM_ENTRIES 256

typedef struct regs {
  uint32_t gs, fs, es, ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t idt_index, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*interrupt_handler_t)(struct regs* r);

#include "../system.h"
#include <stdbool.h>

static interrupt_handler_t interrupt_handlers[IDT_NUM_ENTRIES];

/*
IRQ ack
*/
void irq_ack(int irq_no) {
	if (irq_no >= 12) {
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
}

/*
Enables IRQ
*/
void enable_irq(uint8_t irq) {
	uint8_t mask_master, mask_slave;

	mask_master = inb(0x21);
	mask_slave = inb(0xa1);

	if (irq > 7) {
		outb(0xa1, mask_slave & ~(1 << (irq-8)));
	} else {
		outb(0x21, mask_master & ~(1 << irq));
	}
}

/*
Registers interrupt handler
*/
bool register_interrupt_handler(uint32_t idt_index, interrupt_handler_t handler) {
    if (idt_index >= IDT_NUM_ENTRIES) {
        return false;
    }

    if (interrupt_handlers[idt_index] != 0) {
        return false;
    }

    interrupt_handlers[32 + idt_index] = handler;
    return true;
}

/*
IRQ handler
*/
void irq_handler(struct regs *r) {
    void (*handler)(struct regs * r);

    handler = interrupt_handlers[r->idt_index];
    if (handler) {
        handler(r);
    }

    if (r->idt_index >= 40) {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}

/*
Runs interrupt handler
*/
void run_interrupt_handler(struct regs* r) {
    uint32_t idt_index = r->idt_index;
    if (idt_index < 32) {
        return;
    }

    if (idt_index >= 32 && idt_index <= 47) {
        irq_handler(r);
        return;
    }

    if (interrupt_handlers[r->idt_index] != 0) {
        interrupt_handlers[r->idt_index](r);
    }
}

#endif
