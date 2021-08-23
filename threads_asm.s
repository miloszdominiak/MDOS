.global thread_switch

.extern current_thread
.extern thread_running
.extern thread_ready

.section .text
thread_switch:
    push %ebx
    push %esi
    push %edi
    push %ebp

    

    mov current_thread, %edi
    mov %esp, (%edi)

    call thread_ready
    mov (5 * 4)(%esp), %esi
    mov %esi, current_thread

    mov (%esi), %esp

    call thread_running

    pop %ebp
    pop %edi
    pop %esi
    pop %ebx

    ret
