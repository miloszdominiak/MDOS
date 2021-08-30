[bits 16]
org 0x7e00
 
idt_real:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dd 0			; Real Mode IVT @ 0x0000
 
; Entry16:
;         ; We are already in 16-bit mode here!
 
 	cli			; Disable interrupts.
 
	; Need 16-bit Protected Mode GDT entries!
	mov eax, 0x20	; 16-bit Protected Mode data selector.
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax
 
    mov eax, cr0
    and eax, 0x7FFFFFFe	; Disable paging bit & disable 16-bit pmode.
	mov cr0, eax

 	jmp 0:GoRMode		; Perform Far jump to set CS.
 
 GoRMode:
	mov sp, 0x8000		; pick a stack pointer.
	mov ax, 0		; Reset segment registers to 0.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	lidt [idt_real]
     sti	
    mov ah, 0x05
    mov al, 0x0
    int 0x10
    mov ah, 0x0a
    mov al, 0x2f
    mov bh, 0
    mov cx, 1
    int 0x10


chuj:
    jmp chuj