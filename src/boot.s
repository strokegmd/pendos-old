/*
; GRUB Flags
; 0x00 = Text mode
; 0x04 = Graphics mode
*/

.set FLAGS,    0x00
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0, 0

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	push %ebx
	call kmain
	cli
1:	hlt
	jmp 1b

.size _start, . - _start

.global gdt_flush     # Allows the C code to link to this
gdt_flush:
    cli
    mov 4(%esp), %eax
    lgdt (%eax)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    jmp $0x08, $.flush
.flush:
    ret

.global idt_load
idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

.macro no_error_code_handler idt_index
.global interrupt_handler_\idt_index
interrupt_handler_\idt_index:
    cli
    push $0
    push $\idt_index
    jmp  default_interrupt_handler
.endm

.macro error_code_handler idt_index
.global interrupt_handler_\idt_index
interrupt_handler_\idt_index:
    cli
    push $\idt_index
    jmp  default_interrupt_handler
.endm

.extern irq_handler

default_interrupt_handler:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax  # Push us the stack
    push %eax
    call run_interrupt_handler # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp   # Cleans up the pushed error code and pushed ISR number
    sti
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

# ISRs
no_error_code_handler 0
no_error_code_handler 1
no_error_code_handler 2
no_error_code_handler 3
no_error_code_handler 4
no_error_code_handler 5
no_error_code_handler 6
no_error_code_handler 7
error_code_handler 8
no_error_code_handler 9
error_code_handler 10
error_code_handler 11
error_code_handler 12
error_code_handler 13
error_code_handler 14
no_error_code_handler 15
no_error_code_handler 16
error_code_handler 17
no_error_code_handler 18
no_error_code_handler 19

# IRQs
no_error_code_handler 32
no_error_code_handler 33
no_error_code_handler 34
no_error_code_handler 35
no_error_code_handler 36
no_error_code_handler 37
no_error_code_handler 38
no_error_code_handler 39
no_error_code_handler 40
no_error_code_handler 41
no_error_code_handler 42
no_error_code_handler 43
no_error_code_handler 44
no_error_code_handler 45
no_error_code_handler 46
no_error_code_handler 47

.type outsw, @function
.global outsw
outsw:
    pushl   %esi
    movl    8(%esp), %edx  # port
    movl    12(%esp), %esi # addr
    movl    16(%esp), %ecx # count

    xorl    %eax, %eax
.outsw_startLoop:
    cmpl    %eax, %ecx
    je      .outsw_end

    outsw

    incl    %eax
    jmp     .outsw_startLoop

.outsw_end:
    popl    %esi
    ret

.type insw, @function
.global insw
insw:
    pushl   %edi
    movl    8(%esp), %edx  # port
    movl    12(%esp), %edi # addr
    movl    16(%esp), %ecx # count

    xorl    %eax, %eax
.insw_startLoop:
    cmpl    %eax, %ecx
    je      .insw_end

    insw

    incl    %eax
    jmp     .insw_startLoop

.insw_end:
    popl    %edi
    ret

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top: