.extern ps2_keyboard_interrupt

.global irq1
.global unmaskable

.global load_idt

.section .text
load_idt:
	lidt 4(%esp)
	sti
	ret

irq1:
    pusha
    call ps2_keyboard_interrupt
    popa
    iret

unmaskable:
    iret