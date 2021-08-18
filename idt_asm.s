.extern isr_table
.extern stub_table

.global load_idt

.macro stub number
.global isr_stub\number
isr_stub\number:
    pusha
    mov (isr_table + 4 * \number), %eax
    call *%eax
    popa
    iret
.endm

.macro load_stub number
    lea isr_stub\number, %eax
    mov %eax, (stub_table + 4 * \number)
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
