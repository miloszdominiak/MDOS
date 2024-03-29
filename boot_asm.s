.set MAGIC, 0x1BADB002
.set FLAGS, 4
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 1280
    .long 720
    .long 32

.section .bss
    .align 16

stack_bottom:
    .skip 16384
stack_top:

.section .rodata
.global sperma
sperma:

.global freememory
freememory:

.section .text
.global _start

_start:
    mov $stack_top, %esp
    push %ebx
    call kernel_main
    cli
    
end_loop:
    hlt
    jmp end_loop
