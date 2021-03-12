.global load_gdt

.section .text
load_gdt:
    lgdt 4(%esp)

    jmp $0x08,$reload
reload: 
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ret