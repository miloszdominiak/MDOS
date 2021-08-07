.extern ps2_keyboard_interrupt
.extern isr_pointer

.global load_idt

.macro stub number
.global isr_stub\number
isr_stub\number:
    pusha
    call ps2_keyboard_interrupt
    popa
    iret
.endm

.macro load_stub number
    lea isr_stub\number, %eax
    mov %eax, (isr_pointer + 4 * \number)
.endm

.section .text
load_idt:
.altmacro
.set i, 0
.rept 256
    load_stub %i
    .set i, i + 1
.endr
.noaltmacro
	lidt 4(%esp)
	sti
	ret

.altmacro

.set i, 0
.rept 256
    stub %i
    .set i, i + 1
.endr