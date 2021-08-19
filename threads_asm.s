.global switch_thread
.extern current_thread_info

.section .text
switch_thread:
    push %ebx
    push %esi
    push %edi
    push %ebp

    mov current_thread_info, %edi
    mov %esp, (%edi)

    mov (5 * 4)(%esp), %esi
    mov %esi, current_thread_info

    mov (%esi), %esp

    pop %ebp
    pop %edi
    pop %esi
    pop %ebx

    sti
    ret