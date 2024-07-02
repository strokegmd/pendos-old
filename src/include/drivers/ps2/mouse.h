#ifndef __MOUSE_H
#define __MOUSE_H

#include "../../system.h"
#include "../../cpu/irq.h"
#include "../../lib/math.h"

uint8_t mouse_cycle = 0;
int8_t  mouse_byte[3];

int32_t mouse_x = 400;
int32_t mouse_y = 300;
uint8_t mouse_button = 0;
uint8_t mouse_button_prev = 0;

int32_t mouse_xd = 0; // xd))))) d means difference tipo da ponyal?
int32_t mouse_yd = 0;

void mouse_write(uint8_t write) {
	outb(0x64, 0xD4);
	outb(0x60, write);
    inb(0x60);
}

bool mouse_is_moving() {
	return mouse_xd != 0 || mouse_yd != 0;
}

void mouse_handler(struct regs *r) {
	uint8_t status = inb(0x64);
	while (status & 0x01) {
		int8_t mouse_in = inb(0x60);
		if (status & 0x20) {
			switch (mouse_cycle) {
				case 0:
					mouse_byte[0] = mouse_in;
					if (!(mouse_in & 0x08)) return;
					++mouse_cycle;
					break;
				case 1:
					mouse_byte[1] = mouse_in;
					++mouse_cycle;
					break;
				case 2:
					mouse_byte[2] = mouse_in;
					if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) break;
					mouse_xd = mouse_byte[1];
					mouse_yd = mouse_byte[2];
                    mouse_x += mouse_xd;
                    mouse_y -= mouse_yd;
					mouse_button_prev = mouse_button;
					mouse_button = 0;
                    if (mouse_byte[0] & 0x01) mouse_button = 0x01;
                    if (mouse_byte[0] & 0x02) mouse_button = 0x02;
                    if (mouse_byte[0] & 0x04) mouse_button = 0x03;
					mouse_cycle = 0;
					break;
			}
		}
		status = inb(0x64);
	}
}

void mouse_install() {
	uint8_t status;
	outb(0x64, 0xA8);
	outb(0x64, 0x20);
	status = inb(0x60) | 2;
	outb(0x64, 0x60);
	outb(0x60, status);
	mouse_write(0xF6);
	mouse_write(0xF4);
    register_interrupt_handler(12, mouse_handler);
}

#endif
