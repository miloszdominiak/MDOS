.global outb
.global outd
.global inb
.global inw
.global ind
.global chuj

.section .text
outb:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %dx
    mov 12(%ebp), %ax
    out %al, %dx
    leave
    ret

outd:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %dx
    mov 12(%ebp), %eax
    out %eax, %dx
    leave
    ret

inb:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %dx
    in %dx, %al
    leave
    ret

inw:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %dx
    in %dx, %ax
    leave
    ret

ind:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %dx
    in %dx, %eax
    leave
    ret
