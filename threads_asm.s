.global thread_switch
.extern current_thread

.section .text
thread_switch:
    push %ebx
    push %esi
    push %edi
    push %ebp

    mov current_thread, %edi
    mov %esp, (%edi)

    mov (5 * 4)(%esp), %esi
    mov %esi, current_thread

    mov (%esi), %esp

    pop %ebp
    pop %edi
    pop %esi
    pop %ebx

    sti
    ret