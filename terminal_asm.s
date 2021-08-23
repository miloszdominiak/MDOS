.section .text
.global disable_cursor

disable_cursor:
    mov $0x3D4, %dx
    mov $0xA, %al
    outb %al, %dx

    inc %dx
    mov $0x20, %al
    outb %al, %dx