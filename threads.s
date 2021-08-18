.extern ten_proces
.extern drugi_proces
.extern drugi_stack
.extern drugi_proces_funkcja
.extern sperma

.global magic
.global wywolywacz_spermy

.section .text
magic:
    mov %esp, %eax
    mov $drugi_stack, %esp
    call jaba
    call sperma

jaba:
    mov %esp, %ebx
    mov %eax, %esp
    ret

wywolywacz_spermy:
    push $sperma
    ret
